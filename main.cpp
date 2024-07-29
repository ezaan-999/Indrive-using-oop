#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;
const int MAX_CITIES = 5;

class Ride
{
public:
	string customerID;
	string fromCity;
	string toCity;
	double distance;
	double fare;
	string status;

	Ride(string cID, string from, string to, double dist, double f)
	{
		customerID = cID;
		fromCity = from;
		toCity = to;
		distance = dist;
		fare = f;
		status = "Pending";
	}
};

class User
{
protected:
	string userID;
	string password;
	string userType;

public:
	// ---------------Constructor initializing member variables
	User()
	{
		userID = "";
		password = "";
		userType = "User";
	}

	User(string id, string pass, string type)
	{
		userID = id;
		password = pass;
		userType = type;
	}
	// ---------------Pure virtual function for polymorphism
	virtual void displayMenu() = 0;

	// ---------------Getter methods for encapsulation
	string getUserID()
	{
		return userID;
	}
	string getPassword()
	{
		return password;
	}
	string getUserType()
	{
		return userType;
	}
};

class Admin : public User // admin can change user and
{
public:
	Admin(string id, string pass)
	{
		userID = id;
		password = pass;
		userType = "Admin";
	}
	// ---------------Overridden function demonstrating polymorphism
	void displayMenu() override
	{
		cout << "Admin Menu:\n";
		cout << "1. Remove User\n";
		cout << "2. Logout\n";
	}
	// ---------------Function demonstrating file handling and encapsulation
	void removeUser(const string& id)
	{
		ifstream input("records.txt");
		ofstream temp("temp.txt");

		if (!input.is_open())// it check if the file is open or not
		{
			cout << "Error opening records.txt for reading." << endl;
			return;
		}
		if (!temp.is_open())
		{
			cout << "Error opening temp.txt for writing." << endl;
			input.close();
			return;
		}

		string uid, upass, utype;
		bool found = false;

		while (input >> uid >> upass >> utype)
		{
			if (uid == id)
			{
				found = true;
				continue;
			}
			temp << uid << " " << upass << " " << utype << endl;
		}

		input.close();
		temp.close();

		if (found) {
			cout << "\t\t\tUser " << id << " removed successfully.\n";
		}
		else {
			cout << "User " << id << " not found.\n";
			remove("temp.txt");
			return;
		}

		ifstream tempInput("temp.txt");
		ofstream output("records.txt");

		if (!tempInput.is_open())
		{
			cout << "Error opening temp.txt for reading." << endl;
			return;
		}
		if (!output.is_open())
		{
			cout << "Error opening records.txt for writing." << endl;
			tempInput.close();
			return;
		}

		string line;
		while (getline(tempInput, line))
		{
			output << line << endl;
		}
		tempInput.close();
		output.close();
		remove("temp.txt");
	}


};

class City
{
public:
	string name;
	double x, y;

	City()
	{
		name = "";
		x = 0;
		y = 0;
	}

	City(string n, double xCoord, double yCoord)
	{
		name = n;
		x = xCoord; //cities cordinate
		y = yCoord; //
	}
};

class Trip
{
public:
	City fromCity;
	City toCity;
	double distance;
	double fare;

	Trip()
	{
		distance = 0;
		fare = 0;
	}

	Trip(City from, City to)
	{
		fromCity = from;
		toCity = to;

		distance = calculateDistance(fromCity.x, fromCity.y, toCity.x, toCity.y);
		fare = distance * 50;
	}

	static double calculateDistance(double x1, double y1, double x2, double y2)
	{
		return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)); // Euclidean distance
	}
};

class Courier
{
public:
	City fromCity;
	City toCity;
	double weight;
	double distance;
	double fare;

	Courier()
	{
		weight = 0.0;
		distance = 0.0;
		fare = 0.0;
	}
	Courier(City from, City to, double wt)
	{
		fromCity = from;
		toCity = to;
		weight = wt;
		distance = Trip::calculateDistance(fromCity.x, fromCity.y, toCity.x, toCity.y);
		fare = distance * 30 + weight * 10;
	}
};

class Rickshaw : public Trip
{
public:
	Rickshaw(City from, City to) : Trip(from, to)
	{
		fare = distance * 30;
	}
};

class Bike : public Trip
{
public:
	Bike(City from, City to) : Trip(from, to)
	{
		fare = distance * 15;
	}
};

class ComfortWithAC : public Trip
{
public:
	ComfortWithAC(City from, City to) : Trip(from, to)
	{
		fare = distance * 100;
	}
};

class Rider : public User
{
public:
	Rider(string id, string pass)
	{
		userID = id;
		password = pass;
		userType = "Rider";
	}

	void displayMenu() override
	{
		cout << "\t\t\tRider Menu:"
			<<"\t\t\t\n1.View Rides"
			<<"\t\t\t\n2.Accept Ride"
			<<"\t\t\t\n3.Complete Ride"
			<<"\t\t\t\n4.Logout\n";
	}

	void viewRides()
	{
		ifstream inFile("rides.txt");
		if (inFile.is_open())
		{
			string customerID, fromCity, toCity, status;
			double distance, fare;
			cout << "\t\t\tRides List:\n";
			while (inFile >> customerID >> fromCity >> toCity >> distance >> fare >> status)
			{
				cout << "Customer ID: " << customerID << ", From: " << fromCity
					<< ", To: " << toCity << ", Distance: " << distance
					<< " km, Fare: " << fare << " units, Status: " << status << "\n";
			}
			inFile.close();
		}
		else
		{
			cout << "\t\t\tUnable to open rides.txt for reading." << endl;
		}
	}
	void acceptRide(string customerID)
	{
		ifstream inFile("rides.txt");
		ofstream tempFile("temp.txt");

		if (!inFile.is_open())
		{
			cout << "Error opening rides.txt for reading." << endl;
			return;
		}
		if (!tempFile.is_open())
		{
			cout << "Error opening temp.txt for writing." << endl;
			inFile.close();
			return;
		}

		string cid, from, to, status;
		double dist, fare;
		bool found = false;

		while (inFile >> cid >> from >> to >> dist >> fare >> status) {
			if (customerID == cid && status == "Pending") {
				status = "Accepted";
				found = true;
			}
			tempFile << cid << " " << from << " " << to << " " << dist << " " << fare << " " << status << endl;
		}

		inFile.close();
		tempFile.close();

		if (!found) {
			cout << "Ride not found or already accepted." << endl;
			remove("temp.txt");
			return;
		}

		remove("rides.txt");
		rename("temp.txt", "rides.txt");
		cout << "Ride accepted successfully." << endl;
	}

	void completeRide(string customerID)
	{
		ifstream inFile("rides.txt");
		ofstream tempFile("temp.txt");

		if (!inFile.is_open())
		{
			cout << "Error opening rides.txt for reading." << endl;
			return;
		}
		if (!tempFile.is_open())
		{
			cout << "Error opening temp.txt for writing." << endl;
			inFile.close();
			return;
		}

		string cid, from, to, status;
		double dist, fare;
		bool found = false;

		while (inFile >> cid >> from >> to >> dist >> fare >> status) {
			if (customerID == cid && status == "Accepted") {
				status = "Completed";
				found = true;
			}
			tempFile << cid << " " << from << " " << to << " " << dist << " " << fare << " " << status << endl;
		}

		inFile.close();
		tempFile.close();

		if (!found)
		{
			cout << "\tRide not found or not in accepted status." << endl;
			remove("temp.txt");
			return;
		}

		remove("rides.txt");
		rename("temp.txt", "rides.txt");
		cout << "\tRide completed successfully." << endl;
	}
};

class Customer : public User
{
public:
	Customer(string id, string pass)
	{
		userID = id;
		password = pass;
		userType = "Customer";
	}

	void displayMenu() override
	{
		cout << "\t\t\tCustomer Menu:\n"
			<<"\t\t\t1. Book Ride\n"
			<<"\t\t\t2. Book Courier\n"
			<<"\t\t\t3. Logout\n";
	}

	// ---------------Function demonstrating composition, abstraction, and user interaction
	void bookRide(City cities[], int numCities)
	{
		string fromCityName, toCityName;
		cout << "\t\t\tEnter the starting city: ";
		cin >> fromCityName;
		cout << "\t\t\tEnter the destination city: ";
		cin >> toCityName;

		City fromCity, toCity;
		bool fromCityFound = false, toCityFound = false;

		for (int i = 0; i < numCities; ++i)
		{
			if (cities[i].name == fromCityName)
			{
				fromCity = cities[i];
				fromCityFound = true;
			}
			if (cities[i].name == toCityName)
			{
				toCity = cities[i];
				toCityFound = true;
			}
		}

		if (fromCityFound && toCityFound)
		{
			int rideType;
			while (true) {
				cout << "\t\t\tSelect ride type:\n";
				cout << "\t\t\t1. Car\n";
				cout << "\t\t\t2. Rickshaw\n";
				cout << "\t\t\t3. Bike\n";
				cout << "\t\t\t4. Comfort Car with AC\n\t\t\t";
				cin >> rideType;

				// ----------input validation
				if (rideType >= 1 && rideType <= 4)
				{
					break;
				}
				else
				{
					cin.clear();
					while (cin.get() != '\n');
					{
						cout << "Invalid selection. Please choose a valid ride type (1-4).\n";
					}
				}
			}
			Trip* trip = nullptr;
			switch (rideType)
			{
			case 1:
				trip = new Trip(fromCity, toCity);
				break;
			case 2:
				trip = new Rickshaw(fromCity, toCity);
				break;
			case 3:
				trip = new Bike(fromCity, toCity);
				break;
			case 4:
				trip = new ComfortWithAC(fromCity, toCity);
				break;
			default:
				cout << "\t\t\tInvalid ride type selected.\n";
				return;
			}

			if (trip)
			{
				cout << "\t\t\tTrip booked successfully!\n";
				cout << "\t\t\tFrom: " << fromCity.name << endl << "\t\t\tTo: " << toCity.name << "\n";
				cout << "\t\t\tDistance: " << trip->distance << " km\n";
				cout << "\t\t\tFare: " << trip->fare << " Rupees\n";
				cout << "\t\t\tYour rider is on the way :)" << endl;
				Ride ride(userID, fromCity.name, toCity.name, trip->distance, trip->fare);
				logRide(ride);

				delete trip;
			}
		}
		else
		{
			cout << "\t\t\tInvalid cities entered.\n";
		}
	}

	void logRide(const Ride& ride)
	{
		ofstream outFile("rides.txt", ios::app);
		if (outFile.is_open())
		{
			outFile << ride.customerID << " "
				<< ride.fromCity << " "
				<< ride.toCity << " "
				<< ride.distance << " "
				<< ride.fare << " "
				<< "Pending"
				<< endl;
			outFile.close();
		}
		else
		{
			cout << "\t\t\tUnable to open rides.txt for logging." << endl;
		}
	}

	void bookCourier(City cities[], int numCities)
	{
		string fromCityName, toCityName;
		double weight;
		cout << "\t\t\tEnter the starting city: ";
		cin >> fromCityName;
		cout << "\t\t\tEnter the destination city: ";
		cin >> toCityName;
		cout << "\t\t\tEnter the weight of the package (kg): ";
		cin >> weight;

		City fromCity, toCity;
		bool fromCityFound = false, toCityFound = false;

		for (int i = 0; i < numCities; ++i)
		{
			if (cities[i].name == fromCityName)
			{
				fromCity = cities[i];
				fromCityFound = true;
			}
			if (cities[i].name == toCityName)
			{
				toCity = cities[i];
				toCityFound = true;
			}
		}

		if (fromCityFound && toCityFound)
		{

			Courier courier(fromCity, toCity, weight);
			cout << "\t\t\tCourier booked successfully!\n";
			cout << "\t\t\tFrom: " << fromCity.name << " To: " << toCity.name << "\n";
			cout << "\t\t\tDistance: " << courier.distance << " km\n";
			cout << "\t\t\tWeight: " << weight << " kg\n";
			cout << "\t\t\tFare: " << courier.fare << " units\n";
		}
		else
		{
			cout << "\t\t\tInvalid cities entered.\n";
		}
	}

	// ---------------Function demonstrating file handling and encapsulation
	void logRides(const Ride& ride)
	{
		ofstream outFile("rides.txt", ios::app);
		if (outFile.is_open())
		{
			outFile << ride.customerID << " "
				<< ride.fromCity << " "
				<< ride.toCity << " "
				<< ride.distance << " "
				<< ride.fare << " "
				<< "Pending" << endl;
			outFile.close();
		}
		else
		{
			cout << "Unable to open rides.txt for logging." << endl;
		}
	}
};

void login();
void registration();
void forgot();
void adminMenu(Admin& admin);
void riderMenu(Rider& rider);
void customerMenu(Customer& customer, City cities[], int numCities);

City cities[5] = {
	City("Karachi", 24.8607, 67.0011),
	City("Lahore", 31.5497, 74.3436),
	City("Islamabad", 33.6844, 73.0479),
	City("Quetta", 30.1798, 66.9750),
	City("Peshawar", 34.0151, 71.524)
};

// ---------------main() function demonstrating user input etc
int main()
{
	int choice;
	do
	{
		while (true) {
			cout << "\t\t\t---------------------------------\n";
			cout << "\t\t\t         ____________" << endl;
			cout << "\t\t\t        //    ||    \\\\" << endl;
			cout << "\t\t\t _____//||\\\\" << endl;
			cout << "\t\t\t/     _      InDrive   _     (<" << endl;
			cout << "\t\t\t\\/O\\/O\\/" << endl;
			cout << "\t\t\t---------------------------------\n\n";
			cout << "\t\t\tMenu\n\n";
			cout << "\t\t\t|  Press 1 to Register      |" << endl;
			cout << "\t\t\t|  Press 2 to Login         |" << endl;
			cout << "\t\t\t|  Press 3 if you Forgot    |" << endl;
			cout << "\t\t\t|  Press 4 to Exit          |" << endl;
			cout << "\n\t\t\t Please enter your choice: ";
			cin >> choice;

			// ----------input validation
			if (choice >= 1 && choice <= 4)
			{
				break;
			}
			else
			{
				cin.clear();
				while (cin.get() != '\n');
				{
					cout << "Invalid selection. Please choose a valid ride type (1-4).\n";
				}
			}
			cout << endl;
		}
		switch (choice)
		{
		case 1:
			registration();
			break;
		case 2:
			login();
			break;
		case 3:
			forgot();
			break;
		case 4:
			cout << "\t\t\t Thanks for using IN-Drive!   \n\n";
			break;
		default:
			cout << "\t\t\tPlease select a valid option\n" << endl;
		}
	} while (choice != 4);

	return 0;
}

void login()
{
	string userID, password, id, pass, type;
	bool found = false;

	cout << "\t\t\tPlease enter your username and password: " << endl;
	cout << "\t\t\tUSERNAME: ";
	cin >> userID;
	cout << "\t\t\tPASSWORD: ";
	cin >> password;

	ifstream input("records.txt");
	if (input.is_open())
	{
		while (input >> id >> pass >> type)
		{
			if (id == userID && pass == password)
			{
				found = true;
				if (type == "Admin")
				{
					Admin admin(userID, password);
					adminMenu(admin);
				}
				else if (type == "Rider")
				{
					Rider rider(userID, password);
					riderMenu(rider);
				}
				else if (type == "Customer")
				{
					Customer customer(userID, password);
					customerMenu(customer, cities, MAX_CITIES);
				}
				break;
			}
		}
		input.close();
	}
	else
	{
		cout << "\t\t\tError opening file for reading!\n";
	}

	if (!found)
	{
		cout << "\t\t\nLogin error\nPlease check your username and password\n";
	}
}

void registration()
{
	string ruserID, rpassword, rtype;
	int userTypeChoice;
	while (true) {
		cout << "\t\t\tEnter your username: ";
		cin >> ruserID;
		cout << "\t\t\tEnter your password: ";
		cin >> rpassword;
		cout << "\t\t\tSelect user type (1. Admin, 2. Rider, 3. Customer): ";
		cin >> userTypeChoice;

		// ----------input validation
		if (userTypeChoice >= 1 && userTypeChoice <= 3)
		{
			break;
		}
		else
		{
			cin.clear();
			while (cin.get() != '\n');
			{
				cout << "Invalid selection. Please choose a valid ride type (1-3).\n";
			}
		}
	}
	if (userTypeChoice == 1)
	{
		rtype = "Admin";
	}
	else if (userTypeChoice == 2)
	{
		rtype = "Rider";
	}
	else if (userTypeChoice == 3)
	{
		rtype = "Customer";
	}
	else
	{
		cout << "\t\t\tInvalid user type selected!\n";
		return;
	}

	ofstream f1("records.txt", ios::app);
	if (f1.is_open())
	{
		f1 << ruserID << " " << rpassword << " " << rtype << endl;
		f1.close();
		cout << "\t\t\tRegistration successful!\n";
	}
	else
	{
		cout << "\t\t\tError opening file for writing!\n";
	}
}

void forgot()
{
	int option;
	while (true) {
		cout << "\t\t\tForgot Password?\n";
		cout << "Press 1 to search your ID by username\n";
		cout << "Press 2 to go back to the main menu\n";
		cout << "\t\t\tEnter your choice: ";
		cin >> option;

		// ----------input validation
		if (option >= 1 && option <= 2)
		{
			break;
		}
		else
		{
			cin.clear();
			while (cin.get() != '\n');
			{
				cout << "Invalid selection. Please choose a valid ride type (1 or 2).\n";
			}
		}
	}
	switch (option)
	{
	case 1:
	{
		string suserID, sID, spass, stype;
		bool found = false;

		cout << "\n\t\tEnter the last username you remembered: ";
		cin >> suserID;

		ifstream f2("records.txt");
		if (f2.is_open())
		{
			while (f2 >> sID >> spass >> stype)
			{
				if (sID == suserID)
				{
					found = true;
					break;
				}
			}
			f2.close();
		}
		else
		{
			cout << "\nFile not opened!\n";
		}

		if (found)
		{
			cout << "\n\nYour account is found!\n";
			cout << "Your password is: " << spass << endl;
		}
		else
		{
			cout << "\n\tSorry, your account is not found\n";
		}
		break;
	}
	case 2:
		break;
	default:
		cout << "\t\t\tInvalid choice, please try again!\n";
		forgot();
		break;
	}
}

void adminMenu(Admin& admin)
{
	int choice;
	do
	{
		while (true) {
			admin.displayMenu();
			cout << "\t\t\tPlease enter your choice: ";
			cin >> choice;

			// ----------input validation
			if (choice >= 1 && choice <= 2)
			{
				break;
			}
			else
			{
				cin.clear();
				while (cin.get() != '\n');
				{
					cout << "Invalid selection. Please choose a valid ride type (1 or 2).\n";
				}
			}
		}
		switch (choice)
		{
		case 1:
		{
			string userIDToRemove;
			cout << "\t\t\tEnter the username to remove: ";
			cin >> userIDToRemove;
			admin.removeUser(userIDToRemove);
			break;
		}
		case 2:
		{
			cout << "\t\t\tLogging out...\n";
			return;
		}
		default:
			cout << "\t\t\tInvalid choice, please try again.\n";
		}
	} while (true);
}

void riderMenu(Rider& rider)
{
	int choice;
	do
	{
		while (true) {
			rider.displayMenu();
			cout << "\t\t\tPlease enter your choice: ";
			cin >> choice;

			// ----------input validation
			if (choice >= 1 && choice <= 4)
			{
				break;
			}
			else
			{
				cin.clear();
				while (cin.get() != '\n');
				{
					cout << "Invalid selection. Please choose a valid ride type (1-4).\n";
				}
			}

		}
		switch (choice)
		{
		case 1:
			rider.viewRides();
			break;
		case 2:
		{
			string customerID;
			cout << "\t\t\tEnter customer ID for the ride to accept: ";
			cin >> customerID;
			rider.acceptRide(customerID);
			break;
		}
		case 3:
		{
			string customerID;
			cout << "\t\t\tEnter customer ID for the ride to complete: ";
			cin >> customerID;
			rider.completeRide(customerID);
			break;
		}
		case 4:
			cout << "\t\t\tLogging out...\n";
			return;
		default:
			cout << "\t\t\tInvalid choice, please try again.\n";
		}
	} while (true);
}

void customerMenu(Customer& customer, City cities[], int numCities)
{
	int choice;
	do
	{
		while (true)
		{
			customer.displayMenu();
			cout << "\t\t\tPlease enter your choice: ";
			cin >> choice;

			// ----------input validation
			if (choice >= 1 && choice <= 4)
			{
				break;
			}
			else
			{
				cin.clear();
				while (cin.get() != '\n');
				{
					cout << "Invalid selection. Please choose a valid ride type (1-4).\n";
				}
			}
		}
		switch (choice)
		{
		case 1:
			customer.bookRide(cities, numCities);
			break;
		case 2:
			customer.bookCourier(cities, numCities);
			break;
		case 3:
			cout << "\t\t\tLogging out...\n";
			return;
		default:
			cout << "\t\t\tInvalid choice, please try again.\n";
		}
	}while(true);
}
