CREATE TABLE jbmanager (
id integer primary key,
bonus integer default 0 not null,
constraint fk_mgr_emp FOREIGN KEY (id) references jbemployee(id)
);
-- Query OK, 0 rows affected (0.02 sec)

desc jbmanager;
-- +-------+---------+------+-----+---------+-------+
-- | Field | Type    | Null | Key | Default | Extra |
-- +-------+---------+------+-----+---------+-------+
-- | id    | int(11) | NO   | PRI | NULL    |       |
-- | bonus | int(11) | NO   |     | 0       |       |
-- +-------+---------+------+-----+---------+-------+

INSERT INTO jbmanager (id)
SELECT jbemployee.id FROM jbemployee
WHERE jbemployee.id IN (SELECT manager FROM jbemployee)
OR jbemployee.id IN (SELECT manager FROM jbdept);
-- Query OK, 12 rows affected (0.01 sec)
-- Records: 12  Duplicates: 0  Warnings: 0

select * from jbmanager;
-- +-----+-------+
-- | id  | bonus |
-- +-----+-------+
-- |  10 |     0 |
-- |  13 |     0 |
-- |  26 |     0 |
-- |  32 |     0 |
-- |  33 |     0 |
-- |  35 |     0 |
-- |  37 |     0 |
-- |  55 |     0 |
-- |  98 |     0 |
-- | 129 |     0 |
-- | 157 |     0 |
-- | 199 |     0 |
-- +-----+-------+
-- 12 rows in set (0.00 sec)

ALTER TABLE jbemployee DROP FOREIGN KEY fk_emp_mgr;
-- Query OK, 25 rows affected (0.04 sec)
-- Records: 25  Duplicates: 0  Warnings: 0
ALTER TABLE jbemployee ADD CONSTRAINT fk_emp_mgr
FOREIGN KEY (manager) REFERENCES jbmanager(id);
-- Query OK, 25 rows affected (0.04 sec)
-- Records: 25  Duplicates: 0  Warnings: 0

ALTER TABLE jbdept DROP FOREIGN KEY fk_dept_mgr;
-- Query OK, 19 rows affected (0.05 sec)
-- Records: 19  Duplicates: 0  Warnings: 0
ALTER TABLE jbdept ADD CONSTRAINT fk_dept_mgr
FOREIGN KEY (manager) REFERENCES jbmanager(id);
-- Query OK, 19 rows affected (0.05 sec)
-- Records: 19  Duplicates: 0  Warnings: 0

select
CONSTRAINT_NAME as CONST, TABLE_NAME as TNAME, COLUMN_NAME as CNAME,
REFERENCED_TABLE_NAME as RTNAME, REFERENCED_COLUMN_NAME as RCNAME
from INFORMATION_SCHEMA.KEY_COLUMN_USAGE
where CONSTRAINT_NAME='fk_emp_mgr' or CONSTRAINT_NAME='fk_dept_mgr';
-- +-------------+------------+---------+-----------+--------+
-- | CONST       | TNAME      | CNAME   | RTNAME    | RCNAME |
-- +-------------+------------+---------+-----------+--------+
-- | fk_dept_mgr | jbdept     | manager | jbmanager | id     |
-- | fk_emp_mgr  | jbemployee | manager | jbmanager | id     |
-- +-------------+------------+---------+-----------+--------+
-- 2 rows in set (0.58 sec)

UPDATE jbmanager SET bonus = bonus + 10000
WHERE jbmanager.id IN (SELECT manager FROM jbdept);
-- Query OK, 11 rows affected (0.01 sec)
-- Rows matched: 11  Changed: 11  Warnings: 0

select * from jbmanager;
-- +-----+-------+
-- | id  | bonus |
-- +-----+-------+
-- |  10 | 10000 |
-- |  13 | 10000 |
-- |  26 | 10000 |
-- |  32 | 10000 |
-- |  33 | 10000 |
-- |  35 | 10000 |
-- |  37 | 10000 |
-- |  55 | 10000 |
-- |  98 | 10000 |
-- | 129 | 10000 |
-- | 157 | 10000 |
-- | 199 |     0 |
-- +-----+-------+
-- 12 rows in set (0.00 sec)

CREATE TABLE jbmanager (
id integer primary key,
bonus integer default 0 not null,
constraint fk_mgr_emp FOREIGN KEY (id) references jbemployee(id)
);

CREATE TABLE jbcustomer (
id integer primary key,
name varchar(20) not null,
address varchar(20) not null,
city integer not null,
constraint fk_cust_city FOREIGN KEY (city) REFERENCES jbcity(id)
);
-- Query OK, 0 rows affected (0.03 sec)

desc jbcustomer;
-- +---------+-------------+------+-----+---------+-------+
-- | Field   | Type        | Null | Key | Default | Extra |
-- +---------+-------------+------+-----+---------+-------+
-- | id      | int(11)     | NO   | PRI | NULL    |       |
-- | name    | varchar(20) | NO   |     | NULL    |       |
-- | address | varchar(20) | NO   |     | NULL    |       |
-- | city    | int(11)     | NO   | MUL | NULL    |       |
-- +---------+-------------+------+-----+---------+-------+
-- 4 rows in set (0.01 sec)

INSERT INTO jbcustomer VALUES (1, 'Tester', 'Somewhere', 10);
-- Query OK, 1 row affected (0.01 sec)

select * from jbcustomer;
-- +----+--------+-----------+------+
-- | id | name   | address   | city |
-- +----+--------+-----------+------+
-- |  1 | Tester | Somewhere |   10 |
-- +----+--------+-----------+------+
-- 1 row in set (0.00 sec)

CREATE TABLE jbaccount (
id integer primary key,
customer integer not null,
balance decimal default 0 not null,
constraint fk_acc_cust FOREIGN KEY (customer) REFERENCES jbcustomer(id)
);
-- Query OK, 0 rows affected (0.03 sec)

desc jbaccount;
-- +----------+---------+------+-----+---------+-------+
-- | Field    | Type    | Null | Key | Default | Extra |
-- +----------+---------+------+-----+---------+-------+
-- | id       | int(11) | NO   | PRI | NULL    |       |
-- | customer | int(11) | NO   | MUL | NULL    |       |
-- | balance  | float   | NO   |     | 0       |       |
-- +----------+---------+------+-----+---------+-------+
-- 3 rows in set (0.01 sec)

INSERT INTO jbaccount (id, customer, balance)
SELECT DISTINCT account, 1, 1000 as balance from jbdebit;
-- Query OK, 5 rows affected (0.01 sec)
-- Records: 5  Duplicates: 0  Warnings: 0

select * from jbaccount;
-- +----------+----------+---------+
-- | id       | customer | balance |
-- +----------+----------+---------+
-- | 10000000 |        1 |    1000 |
-- | 11652133 |        1 |    1000 |
-- | 12591815 |        1 |    1000 |
-- | 14096831 |        1 |    1000 |
-- | 14356540 |        1 |    1000 |
-- +----------+----------+---------+
-- 5 rows in set (0.00 sec)

CREATE TABLE jbtransaction (
id integer primary key,
tdate datetime not null,
employee integer not null,
account integer not null,
amount decimal default 0 not null,
type varchar(20) not null,
constraint fk_trans_acc FOREIGN KEY (account) REFERENCES jbaccount(id),
constraint fk_trans_emp FOREIGN KEY (employee) REFERENCES jbemployee(id)
);
-- Query OK, 0 rows affected (0.03 sec)

desc jbtransaction;
-- +----------+---------------+------+-----+---------+-------+
-- | Field    | Type          | Null | Key | Default | Extra |
-- +----------+---------------+------+-----+---------+-------+
-- | id       | int(11)       | NO   | PRI | NULL    |       |
-- | tdate    | datetime      | NO   |     | NULL    |       |
-- | employee | int(11)       | NO   | MUL | NULL    |       |
-- | account  | int(11)       | NO   | MUL | NULL    |       |
-- | amount   | decimal(10,0) | NO   |     | 0       |       |
-- | type     | varchar(20)   | NO   |     | NULL    |       |
-- +----------+---------------+------+-----+---------+-------+
-- 6 rows in set (0.02 sec)

INSERT INTO jbtransaction (id, tdate, employee, account, amount, type)
SELECT
jbdebit.id,
jbdebit.sdate,
jbdebit.employee,
jbdebit.account,
SUM(jbitem.price * jbsale.quantity) as amount,
'sale' as type
FROM jbsale
INNER JOIN jbdebit ON jbdebit.id = jbsale.debit
INNER JOIN jbitem ON jbitem.id = jbsale.item
GROUP BY jbdebit.id;
-- Query OK, 6 rows affected (0.00 sec)
-- Records: 6  Duplicates: 0  Warnings: 0

select * from jbtransaction;
-- +--------+---------------------+----------+----------+--------+------+
-- | id     | tdate               | employee | account  | amount | type |
-- +--------+---------------------+----------+----------+--------+------+
-- | 100581 | 1995-01-15 12:06:03 |      157 | 10000000 |   2050 | sale |
-- | 100582 | 1995-01-15 17:34:27 |     1110 | 14356540 |   1000 | sale |
-- | 100586 | 1995-01-16 13:53:55 |       35 | 14096831 |  13446 | sale |
-- | 100592 | 1995-01-17 09:35:23 |      129 | 10000000 |    650 | sale |
-- | 100593 | 1995-01-18 12:34:56 |       35 | 11652133 |    430 | sale |
-- | 100594 | 1995-01-18 10:10:10 |      215 | 12591815 |   3295 | sale |
-- +--------+---------------------+----------+----------+--------+------+
-- 6 rows in set (0.01 sec)

ALTER TABLE jbsale
DROP FOREIGN KEY fk_sale_debit,
ADD CONSTRAINT fk_sale_trans
FOREIGN KEY (debit) REFERENCES jbtransaction(id);
-- Query OK, 8 rows affected (0.05 sec)
-- Records: 8  Duplicates: 0  Warnings: 0

select
CONSTRAINT_NAME as CONST, TABLE_NAME as TNAME, COLUMN_NAME as CNAME,
REFERENCED_TABLE_NAME as RTNAME, REFERENCED_COLUMN_NAME as RCNAME
from INFORMATION_SCHEMA.KEY_COLUMN_USAGE
where CONSTRAINT_NAME='fk_sale_trans';
-- +---------------+--------+-------+---------------+--------+
-- | CONST         | TNAME  | CNAME | RTNAME        | RCNAME |
-- +---------------+--------+-------+---------------+--------+
-- | fk_sale_trans | jbsale | debit | jbtransaction | id     |
-- +---------------+--------+-------+---------------+--------+
-- 1 row in set (0.75 sec)

drop table jbdebit;
-- Query OK, 0 rows affected (0.01 sec)