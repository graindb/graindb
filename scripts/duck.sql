# generate knows_names file
CREATE TABLE personids(id BIGINT, name VARCHAR, gender VARCHAR, age INTEGER, creationDate BIGINT, locationIP VARCHAR, browserUsed VARCHAR, place BIGINT);
CREATE TABLE knowsids(person1id BIGINT, person2id BIGINT, creationDate BIGINT);
COPY personids FROM '/home/g35jin/ldbc_dataset/sf30/person_temp.csv' HEADER DELIMITER '|';
COPY knowsids FROM '/home/g35jin/ldbc_dataset/sf30/knows_temp.csv' HEADER DELIMITER '|';
CREATE TABLE knowsnames AS (SELECT p1.name as p1name, p2.name as p2name, knows.creationDate FROM personids p1, personids p2, knowsids knows WHERE knows.person1id=p1.id AND knows.person2id=p2.id);
.mode list
.output '/home/g35jin/ldbc_dataset/sf30/knows_names.csv'
SELECT * FROM knowsnames ORDER BY p1name;

# execute queries on shuffled knows without index
CREATE TABLE person(name VARCHAR, gender VARCHAR, age INTEGER, creationDate BIGINT, locationIP VARCHAR, browserUsed VARCHAR, place BIGINT);
CREATE TABLE know(p1name VARCHAR, p2name VARCHAR, creationDate BIGINT);
COPY person FROM '/home/g35jin/ldbc_dataset/sf30/person_names.csv' HEADER DELIMITER '|';
COPY know FROM '/home/g35jin/ldbc_dataset/sf30/knows_shuffle.csv' HEADER DELIMITER '|';
SELECT name, gender, p2name FROM person, know WHERE know.p1name=name;
SELECT p1.name, p1.gender, p2.name, p2.gender FROM person p1, person p2, know WHERE p1.name=know.p1name AND p2.name=know.p2name;
SELECT name, gender, p2name FROM person, know WHERE know.p1name=name AND name='Witold Borchardt';
SELECT p1.name, p1.gender, p2.name, p2.gender FROM person p1, person p2, know WHERE p1.name=know.p1name AND p2.name=know.p2name AND p1.name='Witold Borchardt';

# execute queries on shuffled knows with index
CREATE TABLE person(name VARCHAR, gender VARCHAR, age INTEGER, creationDate BIGINT, locationIP VARCHAR, browserUsed VARCHAR, place BIGINT);
CREATE TABLE know(p1name VARCHAR, p2name VARCHAR, creationDate BIGINT);
COPY person FROM '/home/g35jin/ldbc_dataset/sf30/person_names.csv' HEADER DELIMITER '|';
COPY know FROM '/home/g35jin/ldbc_dataset/sf30/knows_shuffle.csv' HEADER DELIMITER '|';
CREATE INDEX know_p1name_idx ON know(p1name);
CREATE INDEX person_name ON person(name);
SELECT name, gender, p2name FROM person, know WHERE know.p1name=name;
SELECT p1.name, p1.gender, p2.name, p2.gender FROM person p1, person p2, know WHERE p1.name=know.p1name AND p2.name=know.p2name;
SELECT name, gender, p2name FROM person, know WHERE know.p1name=name AND name='Witold Borchardt';
SELECT p1.name, p1.gender, p2.name, p2.gender FROM person p1, person p2, know WHERE p1.name=know.p1name AND p2.name=know.p2name AND p1.name='Witold Borchardt';

# execute queries on sorted knows with index
CREATE TABLE person(name VARCHAR, gender VARCHAR, age INTEGER, creationDate BIGINT, locationIP VARCHAR, browserUsed VARCHAR, place BIGINT);
CREATE TABLE know(p1name VARCHAR, p2name VARCHAR, creationDate BIGINT);
COPY person FROM '/home/g35jin/ldbc_dataset/sf30/person_names.csv' HEADER DELIMITER '|';
COPY know FROM '/home/g35jin/ldbc_dataset/sf30/knows_names.csv' HEADER DELIMITER '|';
CREATE INDEX know_p1name_idx ON know(p1name);
CREATE INDEX person_name ON person(name);
SELECT name, gender, p2name FROM person, know WHERE know.p1name=name;
SELECT p1.name, p1.gender, p2.name, p2.gender FROM person p1, person p2, know WHERE p1.name=know.p1name AND p2.name=know.p2name;
SELECT name, gender, p2name FROM person, know WHERE know.p1name=name AND name='Witold Borchardt';
SELECT p1.name, p1.gender, p2.name, p2.gender FROM person p1, person p2, know WHERE p1.name=know.p1name AND p2.name=know.p2name AND p1.name='Witold Borchardt';
