create table custom_player (
pName varchar(50) not null,
pYear smallint not null,
statName varchar(50) not null,
statFormula varchar(50) not null,
stat decimal not null,
primary key (pName, pYear, statName)
);