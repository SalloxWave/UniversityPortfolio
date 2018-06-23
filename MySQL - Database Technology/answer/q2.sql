/*  Abbreviations for tables:
	CreditCard crc
	Ticket tck
	Customer cust
	Country cnt
	Booking book
	Passenger pass
	Airport aprt
	Reservation res
	PassengerReservation passres
	Contact ctc
	Route rut
	Flight fli
	WeeklySchedule wsc
	Year year
	Weekday wday
*/

/* DROP DATABASE lovba497;
CREATE DATABASE lovba497;
USE lovba497; */

/*DROP DATABASE BrianAir;
CREATE DATABASE BrianAir;
USE BrianAir;*/

DROP TABLE IF EXISTS CreditCard;
DROP TABLE IF EXISTS Ticket;
DROP TABLE IF EXISTS Customer;
DROP TABLE IF EXISTS Country;
DROP TABLE IF EXISTS Booking;
DROP TABLE IF EXISTS Passenger;
DROP TABLE IF EXISTS Airport;
DROP TABLE IF EXISTS Reservation;
DROP TABLE IF EXISTS PassengerReservation;
DROP TABLE IF EXISTS Contact;
DROP TABLE IF EXISTS Route;
DROP TABLE IF EXISTS Flight;
DROP TABLE IF EXISTS WeeklySchedule;
DROP TABLE IF EXISTS Year;
DROP TABLE IF EXISTS Weekday;

CREATE TABLE Passenger (
	passportnumber INT primary key not null,
	name VARCHAR(30)
);

CREATE TABLE CreditCard (
	cardnumber BIGINT primary key not null,
	cardholder VARCHAR(30)	
);

CREATE TABLE Year (
	year INT primary key not null,
	cost_factor DOUBLE
);


CREATE TABLE Airport (
	airportcode VARCHAR(3) primary key not null,
	name VARCHAR(30),
	country VARCHAR(30)
);

CREATE TABLE Route (
	airport_departure VARCHAR(3) not null,
	airport_arrival VARCHAR(3) not null,
	year INT not null,
	routeprice DOUBLE,

	primary key (airport_departure, airport_arrival, year),

	constraint fk_rot_aprtd FOREIGN KEY (airport_departure) references Airport(airportcode),
	constraint fk_rot_aprta FOREIGN KEY (airport_arrival) references Airport(airportcode),
	constraint fk_rot_year FOREIGN KEY (year) references Year(year)
);

CREATE TABLE Weekday (
	day VARCHAR(10) not null,
	year INT not null,
	cost_factor DOUBLE,

	primary key (day, year),

	constraint fk_wday_year FOREIGN KEY (year) references Year(year)
);

CREATE TABLE WeeklySchedule (
	id INT primary key auto_increment not null,
	year INT,
	day VARCHAR(10),
	airport_departure VARCHAR(3),
	airport_arrival VARCHAR(3),
	departure_time TIME,

	constraint fk_wsc_year_wday FOREIGN KEY (year) references Weekday(year),
	constraint fk_wsc_year_rut FOREIGN KEY (year) references Route(year),
	constraint fk_wsc_wday FOREIGN KEY (day) references Weekday(day),
	constraint fk_wsc_aprtd FOREIGN KEY (airport_departure) references Route(airport_departure),
	constraint fk_wsc_aprta FOREIGN KEY (airport_arrival) references Route(airport_arrival)
);

CREATE TABLE Flight (
	flightnumber INT primary key auto_increment not null,
	weeklyschedule INT,
	week INT,
	seats INT,

	constraint fk_fli_wsc FOREIGN KEY (weeklyschedule) references WeeklySchedule(id)
);

CREATE TABLE Contact (
	passportnumber INT primary key not null,
	phonenumber BIGINT,
	email VARCHAR(30),

	constraint fk_ctc_pass FOREIGN KEY (passportnumber) references Passenger(passportnumber)
);

CREATE TABLE Reservation (
	id INT primary key auto_increment not null,
	flight INT,
	contact INT,

	constraint fk_res_fli FOREIGN KEY (flight) references Flight(flightnumber),
	constraint fk_res_ctc FOREIGN KEY (contact) references Contact(passportnumber)
);

CREATE TABLE Booking (
	reservation INT primary key not null,
	cardnumber BIGINT,
	price INT,

	constraint fk_book_res FOREIGN KEY (reservation) references Reservation(id),
	constraint fk_book_crc FOREIGN KEY (cardnumber) references CreditCard(cardnumber)
);

CREATE TABLE Ticket (
	ticketnumber INT primary key not null,
	passenger INT,
	booking INT,

	constraint fk_tck_pass FOREIGN KEY (passenger) references Passenger(passportnumber),
	constraint fk_tck_book FOREIGN KEY (booking) references Booking(reservation)
);

CREATE TABLE PassengerReservation (
	passenger INT not null,
	reservation INT not null,

	primary key (passenger, reservation),

	constraint fk_passres_pass FOREIGN KEY (passenger) references Passenger(passportnumber),
	constraint fk_passres_res FOREIGN KEY (reservation) references Reservation(id)
);

SELECT table_name FROM information_schema.tables WHERE table_schema="BrianAir";
