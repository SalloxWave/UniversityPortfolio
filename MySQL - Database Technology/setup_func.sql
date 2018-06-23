DROP PROCEDURE IF EXISTS addYear;
DROP PROCEDURE IF EXISTS addDay;
DROP PROCEDURE IF EXISTS addDestination;
DROP PROCEDURE IF EXISTS addRoute;
DROP PROCEDURE IF EXISTS addFlight;
DROP FUNCTION IF EXISTS calculateFreeSeats;
DROP FUNCTION IF EXISTS calculatePrice;
DROP TRIGGER IF EXISTS TriggerTicketNumber;
DROP PROCEDURE IF EXISTS addReservation;
DROP PROCEDURE IF EXISTS addPassenger;
DROP PROCEDURE IF EXISTS addContact;
DROP PROCEDURE IF EXISTS addPayment;
DROP VIEW IF EXISTS allFlights;

SET @MAX_FLIGHT_SEATS:=40;
SET @MAX_INT_SIZE:=2147483647;

delimiter //
CREATE PROCEDURE addYear (year INT, factor DOUBLE)
BEGIN
	INSERT INTO Year (year, cost_factor) VALUES(year, factor);
END;//

CREATE PROCEDURE addDay (year INT, day VARCHAR(10), factor DOUBLE)
BEGIN
	INSERT INTO Weekday (day, year, cost_factor) VALUES(day, year, factor);
END;//

CREATE PROCEDURE addDestination (airport_code VARCHAR(3), name VARCHAR(30), country VARCHAR(30))
BEGIN
	INSERT INTO Airport (airportcode, name, country) VALUES(airport_code, name, country);
END;//

CREATE PROCEDURE addRoute (departure_airport_code VARCHAR(3), arrival_airport_code VARCHAR(3),
						   year INT, routeprice DOUBLE)
BEGIN
	INSERT INTO Route (airport_departure, airport_arrival, year, routeprice)
	VALUES(departure_airport_code, arrival_airport_code, year, routeprice);
END;//

CREATE PROCEDURE addFlight (departure_airport_code VARCHAR(3), arrival_airport_code VARCHAR(3),
						   year INT, day VARCHAR(10), departure_time time)
BEGIN
	DECLARE wsc_id INT;
	DECLARE i INT;

	INSERT INTO WeeklySchedule (year, day, airport_departure, airport_arrival, departure_time)
	VALUES (year, day, departure_airport_code, arrival_airport_code, departure_time);

	SET wsc_id:=LAST_INSERT_ID();

	SET i:=1;
	loop_flight: LOOP
		IF i > 52 THEN
			LEAVE loop_flight;
		END IF;
		INSERT INTO Flight (weeklyschedule, week, seats) VALUES(wsc_id, i, @MAX_FLIGHT_SEATS);
		SET i:=i+1;
	END LOOP;
END;//

CREATE FUNCTION calculateFreeSeats(flightnumber_a INT)
RETURNS INT
BEGIN
    DECLARE free_seats INT;

	SELECT @MAX_FLIGHT_SEATS - COUNT(PassengerReservation.passenger) INTO free_seats
	FROM Booking, Reservation, PassengerReservation
	WHERE Reservation.flight = flightnumber_a
	AND Booking.reservation = Reservation.id
	AND Booking.reservation = PassengerReservation.reservation;

	IF free_seats IS NULL THEN
		RETURN @MAX_FLIGHT_SEATS;
	ELSE
		RETURN free_seats;
	END IF;
END;//

CREATE FUNCTION calculatePrice(flightnumber_a INT)
RETURNS DOUBLE
BEGIN
	DECLARE weekdayfactor DOUBLE;
	DECLARE routeprice_a DOUBLE;
	DECLARE bookedpassengers INT;
	DECLARE profitfactor DOUBLE;

	SELECT routeprice INTO routeprice_a FROM Route
	WHERE airport_departure IN (SELECT airport_departure FROM WeeklySchedule
		WHERE id IN (SELECT weeklyschedule FROM Flight WHERE flightnumber=flightnumber_a))
	AND airport_arrival IN (SELECT airport_arrival FROM WeeklySchedule
		WHERE id IN (SELECT weeklyschedule FROM Flight WHERE flightnumber=flightnumber_a))
	AND year IN (SELECT year FROM WeeklySchedule
		WHERE id IN (SELECT weeklyschedule FROM Flight WHERE flightnumber=flightnumber_a))
    LIMIT 1;

	SELECT cost_factor INTO weekdayfactor FROM Weekday
    WHERE Weekday.day IN
	(SELECT WeeklySchedule.day FROM WeeklySchedule
	WHERE WeeklySchedule.id IN (SELECT weeklyschedule FROM Flight WHERE flightnumber=flightnumber_a))
	AND Weekday.year IN
	(SELECT WeeklySchedule.year FROM WeeklySchedule
	WHERE WeeklySchedule.id IN (SELECT weeklyschedule FROM Flight WHERE flightnumber=flightnumber_a))
	LIMIT 1;

	SET bookedpassengers := @MAX_FLIGHT_SEATS - calculateFreeSeats(flightnumber_a);

	SELECT cost_factor INTO profitfactor FROM Year
	WHERE year IN
	(SELECT year FROM WeeklySchedule WHERE id IN
		(SELECT weeklyschedule FROM Flight WHERE flightnumber=flightnumber_a));

    RETURN ROUND(routeprice_a * weekdayfactor * (bookedpassengers + 1)/40 * profitfactor, 3);
END;//

CREATE TRIGGER TriggerTicketNumber
AFTER INSERT ON Booking
FOR EACH ROW
BEGIN
	DECLARE done INT DEFAULT FALSE;
	DECLARE pass INT;
	DECLARE curpass CURSOR FOR
		SELECT passenger FROM PassengerReservation WHERE reservation = NEW.reservation;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

	OPEN curpass;

	pass_loop: LOOP
		FETCH curpass INTO pass;
		IF done THEN
			LEAVE pass_loop;
		END IF;
		INSERT INTO Ticket (ticketnumber, passenger, booking) VALUES (ROUND(@MAX_INT_SIZE*rand()), pass, NEW.reservation);
	END LOOP;

	CLOSE curpass;
END;//


CREATE PROCEDURE addReservation(departure_airport_code VARCHAR(3), arrival_airport_code VARCHAR(3),
	year_a INT, week_a INT, day_a VARCHAR(10), time_a TIME, number_of_passengers INT, OUT output_reservation_nr INT)
BEGIN
	DECLARE flightnumber_a INT;
	SELECT flightnumber INTO flightnumber_a FROM Flight
	WHERE week=week_a
	AND weeklyschedule IN
	(SELECT id FROM WeeklySchedule
	WHERE airport_departure = departure_airport_code
	AND airport_arrival = arrival_airport_code
	AND year=year_a AND day=day_a AND departure_time=time_a);

	IF FOUND_ROWS() > 0 THEN
		IF number_of_passengers <= calculateFreeSeats(flightnumber_a) THEN
			INSERT INTO Reservation (flight) VALUES (flightnumber_a);
			SET output_reservation_nr:=LAST_INSERT_ID();
		ELSE
			SIGNAL SQLSTATE "45000"
				SET message_text = "There are not enough seats available on the chosen flight";
		END IF;
	ELSE
		SIGNAL SQLSTATE "45000"
			SET message_text = "There exist no flight for the given route, date and time";
	END IF;
END;//

CREATE PROCEDURE addPassenger(reservation_nr INT, passport_number INT, name VARCHAR(30))
BEGIN
	IF NOT (SELECT EXISTS (SELECT 1 FROM Reservation WHERE id=reservation_nr)) THEN
		SIGNAL SQLSTATE "45000"
			SET message_text = "The given reservation number does not exist";
	END IF;

	IF (SELECT EXISTS (SELECT 1 FROM Booking WHERE reservation IN
					  (SELECT id FROM Reservation WHERE id=reservation_nr))) THEN
		SIGNAL SQLSTATE "45000"
			SET message_text = "The booking has already been payed and no futher passengers can be added";
	END IF;

	IF NOT (SELECT EXISTS (SELECT 1 FROM Passenger WHERE passportnumber=passport_number)) THEN
		INSERT INTO Passenger (passportnumber, name) VALUES (passport_number, name);
	END IF;
	INSERT INTO PassengerReservation (passenger, reservation) VALUES(passport_number, reservation_nr);
END;//

CREATE PROCEDURE addContact(reservation_nr INT, passport_number INT, email VARCHAR(30), phone BIGINT)
BEGIN
	DECLARE passcount INT;
	IF NOT (SELECT EXISTS (SELECT 1 FROM Reservation WHERE id=reservation_nr)) THEN
		SIGNAL SQLSTATE "45000"
			SET message_text = "The given reservation number does not exist";
	END IF;

	IF (SELECT EXISTS (SELECT 1 FROM PassengerReservation
			WHERE passenger=passport_number
			AND reservation=reservation_nr)) THEN

		IF NOT (SELECT EXISTS (SELECT 1 FROM Contact WHERE passportnumber=passport_number)) THEN
			INSERT INTO Contact (passportnumber, phonenumber, email) VALUES(passport_number, phone, email);
		END IF;
		UPDATE Reservation SET contact = passport_number WHERE id=reservation_nr;
	ELSE
		SIGNAL SQLSTATE "45000"
			SET message_text = "The person is not a passenger of the reservation";
	END IF;
END;//

CREATE PROCEDURE addPayment (reservation_nr INT, cardholder_name VARCHAR(30), credit_card_number BIGINT)
BEGIN
	DECLARE flightnumber INT;
	DECLARE freeseats INT;
	DECLARE res_seats INT;

	IF NOT (SELECT EXISTS (SELECT 1 FROM Reservation WHERE id=reservation_nr)) THEN
		SIGNAL SQLSTATE "45000"
			SET message_text="The given reservation number does not exist";
	END IF;

	IF NOT (SELECT EXISTS (SELECT 1 FROM Reservation WHERE id=reservation_nr AND contact IS NOT NULL)) THEN
		SIGNAL SQLSTATE "45000"
			SET message_text="The reservation has no contact yet";
	END IF;

	SELECT flight INTO flightnumber FROM Reservation WHERE id=reservation_nr;

	SELECT COUNT(PassengerReservation.passenger) INTO res_seats
	FROM Reservation, PassengerReservation
	WHERE Reservation.flight = flightnumber
	AND Reservation.id = reservation_nr
	AND Reservation.id = PassengerReservation.reservation;

	SET freeseats:=calculateFreeSeats(flightnumber);

	IF res_seats > freeseats THEN
		DELETE FROM PassengerReservation WHERE reservation=reservation_nr;
		DELETE FROM Reservation WHERE id=reservation_nr;
		SIGNAL SQLSTATE "45000"
			SET message_text="There are not enough seats available on the flight anymore, deleting reservation Kappa";
	END IF;

	IF NOT (SELECT EXISTS (SELECT 1 FROM CreditCard WHERE cardnumber=credit_card_number)) THEN
		INSERT INTO CreditCard (cardnumber, cardholder) VALUES(credit_card_number, cardholder_name);
	END IF;

	/*Sleep to allow both session obtaining res_seats
	with same value but then let first session make a booking*/
	/* SELECT "SLEEPING FOR 5 SECONDS...";
	SELECT SLEEP(1); */

	INSERT INTO Booking (reservation, cardnumber, price) VALUES(reservation_nr, credit_card_number, calculatePrice(flightnumber));
END;//

CREATE VIEW allFlights (departure_city_name, destination_city_name,
						departure_time, departure_day, departure_week,
						departure_year, nr_of_free_seats, current_price_per_seat) AS
	SELECT DEP.name, DEST.name,
		   WSC.departure_time, WSC.day, FLI.week, WSC.year,
		   calculateFreeSeats(FLI.flightnumber), calculatePrice(FLI.flightnumber)
	FROM Airport DEP, Airport DEST, WeeklySchedule WSC, Flight FLI, Route ROU
	WHERE FLI.weeklyschedule = WSC.id
	AND WSC.year = ROU.year
	AND WSC.airport_departure = ROU.airport_departure
	AND WSC.airport_arrival = ROU.airport_arrival
	AND ROU.airport_departure = DEP.airportcode
	AND ROU.airport_arrival = DEST.airportcode
;//

delimiter ;

