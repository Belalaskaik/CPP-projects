#include <fstream>
#include <ios>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Person.hpp"
#include "SocialNetwork.hpp"


// Override the >> operator for std::pair so we can import Edges from connections.txt
std::istream & operator>>( std::istream & stream, std::pair<int, int> & nums )
{

  char delimiter = '\0';
  std::pair<int, int> working_nums;
  
  if (stream >> std::ws >> working_nums.first >> std::ws >> delimiter &&
      delimiter == ',' 
      && stream >> std::ws >> working_nums.second) 
    
  {
    nums = std::move(working_nums);
  } 
  else {
    stream.setstate(std::ios::failbit);
  }
 return stream;


}

int main( )
{

  /*
  Part 1 - Setting Things Up
*/

 //
// Task 1 - Complete the operator>> overload for std::pair objects so we can properly import connections.txt below

// Task 2 - Create a SocialNetwork Object

// Task 3 - Import the people.txt file contents into your SocialNetwork object as Nodes.

// Task 4 - Import the connections.txt file contents into your SocialNetwork object as Edges.



// Task 5 - Using std::cout, tell us how many Nodes (people) and how many Edges (connections) there are in the graph.

std::string nodeFile = "people.txt";
std::string edgeFile = "connections.txt";

  // TODO - Construct the graph container
  SocialNetwork network;


  std::ifstream peopleFile( nodeFile );
  if( peopleFile.is_open() )
  {
    for( std::string line; std::getline( peopleFile, line ); /**/ )
    {
      Person pperson;
      
        std::istringstream stream(line);
        
        stream >> pperson;
      if( pperson.isValid())
      {
        network.Add( pperson);
      }
       
    }
    peopleFile.close();
  }



  std::ifstream connectionFile( edgeFile );
  if( connectionFile.is_open() )
  {
    for( std::string line; std::getline( connectionFile, line ); /**/ )
    {
      std::pair<int, int> connection;
      std::istringstream stream(line);
      
      stream >> connection;
      //std::cout<< connection.first << "," << connection.second<< std::endl;

      if(connection.first != connection.second)
      {
        network.Connect( connection.first, connection.second );
      }

    }
    connectionFile.close();
  }
  


  std::cout << "Number of Nodes (people): " << network.graphSize() << std::endl;
  std::cout << "Number of Edges (connections): " << network.edgeCount() << std::endl;


std::cout<<"----------------------------------------------------------------------------------"<<std::endl;
/*
  Part 2 - Output some Text
*/
std::list<Person>YoungPeople;
std::function<bool(Person)> filterAscending = [](Person p) noexcept{
    return p.Age() < 60;
};
YoungPeople = network.findPeople(filterAscending);


std::list<Person>pplOver33;


std::list<Person>AllPeople;
AllPeople = network.allPeople();

// Task 1 - Output all of the people, sorted ascending by Last Name (A-Z)
std::cout<<" Task 1 - Everyone sorted by last name, ascending: "<<std::endl;
AllPeople.sort(Person::lt_lname());
if(AllPeople.empty() == false)
{
  for(Person i: AllPeople)
  {
    std::cout << i.Bio()<<std::endl;
  }
  std::cout<<std::endl;
}
 
// Task 2 - Output all of the people, sorted ascending by age (youngest to oldest)
std::cout<<" Task 2 - Everyone sorted by age, ascending: "<<std::endl;
AllPeople.sort(Person::lt_age());
if(AllPeople.empty() == false)
{
  for(Person i: AllPeople)
  {
    std::cout << i.Bio()<<std::endl;
  }
  std::cout<<std::endl;
}

// Task 3 - Output people whose age is over 33, sorted descending (oldest to youngest)
//gt is oldest to youngest
std::cout<<" Task 3 - People over 33, sorted by age descending: "<<std::endl;
std::function<bool(Person)> filterDescending  = [](Person p) noexcept{
    return p.Age() >33;
};
pplOver33 = network.findPeople(filterDescending);
pplOver33.sort(Person::gt_age());
if(pplOver33.empty() == false)
{
  for(Person i: pplOver33)
  {
    std::cout << i.Bio()<<std::endl;
  }
  std::cout<<std::endl;
}

/*

YoungPeople.sort(Person::gt_age());
if(YoungPeople.empty() == false)
{
  for(Person i: YoungPeople)
  {
    std::cout << i.Bio()<<std::endl;
  }
}

*/



// Task 4 - Prompt the user to search for a person by inputting a first name and a last name, then attempt to find that person and display their bio.
//          If the person is not found, display a "fname lname was not found" message instead.


std::string  fname;
std::string lname;
std::cout<<"Task 4 & 5 - Search for a person and display their bio & friends: "<<std::endl;

std::cout<< "Press <Enter> after inputting each name"<<std::endl<<"First Name: ";
std::cin>>fname;
std::cout<< "Last name: ";
std::cin>> lname;

std::list<Person > result= network.findPerson(fname, lname);



if(result.empty() == false)
{
  std::string pb =   result.front().Bio();
  std::cout <<pb<<std::endl;

  std::cout<<std::endl;
  std::cout<< pb <<std::endl<< result.front().FirstName()<<"'s friends:"<<std::endl;
  network.showFriends(fname,lname);
  std::cout<<std::endl;
}
else{
  std::cout<< fname <<" "<< lname<< " was not found"<<std::endl;

}



 



// Task 5 - In addition to the bio in Task 4, output the person's list of friends if they are found. 

// Task 6 - Prompt the user to search for 2 people by entering their first and last names
std::cout<<"Task 6 -";
std::cout<< "Press <Enter> after inputting each name"<<std::endl;

std::string p1_fname;
std::string p1_lname;
std::string p2_fname;
std::string p2_lname;

//prompt

std::cout<<"Person 1's first name: ";
std::cin>> p1_fname;
std::cout<<"Person 1's last name: ";
std::cin>> p1_lname;

std::cout<<std::endl;
//to store 1st person as a person Obj
  std::list<Person> persons = network.findPerson(p1_fname, p1_lname);
  if (!persons.empty()) {
      Person person1 = persons.front();
      persons.pop_back();
      std::cout<<"Person 2's first name: ";
      std::cin>> p2_fname;

      std::cout<<"Person 2's last name: ";
      std::cin>> p2_lname;
      std::cout<<std::endl;

    persons = network.findPerson(p2_fname, p2_lname);
    if (!persons.empty()) {
      
      //to store 2nd person as a person Obj
      Person person2 = persons.front();


      int degrees_of_separation = network.distanceBetween( person1, person2 );
      std::string name1 = person1.FirstName()+" " + person1.LastName();
      std::string name2 = person2.FirstName() +" "+ person2.LastName();


      //Display the degrees of separation between the two people
      //If they are directly connected, display "name1 and name2 are friends!" instead
        if(degrees_of_separation == 1){
        std::cout<< name1<< " and "<< name2<< " are friends!"<<std::endl;
        }
        else{
          std::cout<<"The degrees of separation between "<<name1<<" and "<<name2<<" are: "<<degrees_of_separation<<std::endl;
            }

      }else {std::cout << "Person 2 not found!" << std::endl;}

  }else {std::cout << "Person 1 not found!" << std::endl;}







  
 




return 0;
}
