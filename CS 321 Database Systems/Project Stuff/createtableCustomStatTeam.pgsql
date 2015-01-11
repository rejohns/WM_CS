create table custom_team (
tName varchar(50) not null,
tYear smallint not null,
statName varchar(50) not null,
statFormula varchar(50) not null,
stat decimal not null,
primary key (tName, tYear, statName)
);