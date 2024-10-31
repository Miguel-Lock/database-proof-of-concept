create table employee (
	employee_ID int identity(3245, 7),
	first_name varchar(20),
	middle_name varchar(20),
	last_name varchar(20),
	password varchar(40) not null,
	PIN numeric(6),
	sec_question varchar(20),
	SSN char(9),
	position varchar(20) not null,
	manager_ID numeric,
	num_hours numeric,
	payrate numeric(4,2),
	street varchar(20),
	apt_num varchar(20),
	city varchar(20),
	state varchar(20),
	zip varchar(20),
	birthday date, 
	payment_method varchar( 20),
primary key (employee_ID),
check (position in ('customer', 'grunt', 'cashier', 'manager', 'admin')),
check (birthday > '1903-12-12'),
check (num_hours <= 80),
check (payrate >= 10.10)
);

insert into employee(first_name, middle_name, last_name, password, PIN, sec_question, SSN, position, manager_ID, num_hours, payrate, street, apt_num, city, state, zip, birthday, payment_method) values ('bob', 'p.', 'cornell', 'password1', 333222, 'elm', 123456666, 'grunt', 222, 5, 12.25, 'elmstrt', 'n23', 'steubenville', 'OH', 12356, '2000-06-28', 'check');
insert into employee(first_name, middle_name, last_name, password, PIN, sec_question, SSN, position, manager_ID, num_hours, payrate, street, apt_num, city, state, zip, birthday, payment_method) values ('charlie', 'oscar', 'conner', 'securerat', 222333, 'birch', 666554444, 'manager', 33, 4, 18.02, 'sunset blvd.', null, 'weirton', 'PA', 23232, '2002-09-15', 'cash');


create table timesheet (
	employee_ID int,
	clock_date date,
	scheduled_start time,
	scheduled_end time,
	scheduled_num_break int,
	scheduled_num_meal int,
	actual_start time,
	actual_end time,
	actual_num_break int,
	actual_num_meal int,
check (scheduled_end > scheduled_start),
check (actual_end > actual_start),
primary key (employee_ID, clock_date, scheduled_start),
foreign key (employee_ID) references employee
);
insert into timesheet values (3245, '2004-11-12', '11:00:00', '19:00:00', 2, 1, '11:23:00', '18:45:00', 2, 1);
insert into timesheet values (3252, '2004-11-12', '09:00:00', '13:00:00', 3, 2, '08:59:00', '13:00:00', 2, 2);
insert into timesheet (employee_ID, clock_date, scheduled_start, scheduled_end, scheduled_num_break, scheduled_num_meal)
values(3252, '2004-11-13', '10:00:00', '18:00:00', 2, 1);

create table sales (
	transaction_ID int identity(1,1),
	employee_ID int,
	time_ordered time,
	cost numeric(5,2),
	payment_method varchar(20) default 'card',
check (payment_method in ('cash', 'check', 'card')),
primary key (transaction_ID),
foreign key (employee_ID) references employee
);
insert into sales(time_ordered, cost, employee_ID, payment_method) values ('12:30:00', 100.00, 3245, 'cash');
insert into sales(time_ordered, cost, employee_ID, payment_method) values ('12:13', 123.00, 3245, 'cash');

create table menu_items (
	menu_item_name varchar(20),
	price numeric(4,2),
	on_meal_exchange bit default 0,
	is_side bit default 0,
	is_breakfast bit default 0,
	is_lunch bit default 1,
	Is_late_night bit default 0,
primary key (menu_item_name)
);
insert into menu_items values ('cheeseburger', 7.50, 1, 0, 0, 1, 0);
insert into menu_items values ('apple', 0.50, 1, 1, 0, 1, 0);

create table ingredients (
	ingredient_name varchar(20),
	in_stock bit,
	low_stock bit,
primary key (ingredient_name),
);
insert into ingredients values ('hamburger bun', 1, 0);
insert into ingredients values ('apple', 1, 1);

create table inventory (
	inventory_ID int,
	ingredient_name varchar(20),
	expire_date date,
	amount int,
primary key (inventory_ID, expire_date),
foreign key (ingredient_name) references ingredients
);
insert into inventory values (239804, 'hamburger bun', '2023-12-31', 50);
insert into inventory values (349809, 'apple', '2023-12-20', 30);

create table transactions (
	transaction_ID int,
	menu_item_name varchar(20),
	quantity int,
foreign key (transaction_ID) references sales,
foreign key (menu_item_name) references menu_items
);
insert into transactions values (1, 'cheeseburger', 2);
insert into transactions values (1, 'apple', 1);

create table menu_ingredient(
	ingredient_name varchar(20),
	menu_item_name varchar(20),
	quantity int,
foreign key (ingredient_name) references ingredients,
foreign key (menu_item_name) references menu_items
);
insert into menu_ingredient values('hamburger bun', 'cheeseburger', 1);
insert into menu_ingredient values('apple','apple',1);

insert into employee (first_name, last_name, password, position) values ('Joe', 'Mama', 'unhackable', 'grunt'); --id for this employee: 3259
insert into timesheet (employee_ID, clock_date, scheduled_start, scheduled_end, scheduled_num_break, scheduled_num_meal) values (3259, '2015-11-22', '11:30:00', '18:00:00', 1, 1);
