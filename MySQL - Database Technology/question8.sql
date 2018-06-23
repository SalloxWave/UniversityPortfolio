/* DROP TABLE IF EXISTS Test;

CREATE TABLE Test(
    id INT auto_increment primary key,
    a VARCHAR(10)
); */

START TRANSACTION;
INSERT INTO Test (a) VALUES("hej");
SELECT * FROM Test;