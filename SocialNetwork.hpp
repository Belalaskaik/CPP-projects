
#include <algorithm>
#include <initializer_list>
#include <list>
#include <map>
#include <utility>
#include <vector>

#include "Person.hpp"

class SocialNetwork
{
  /*
    Private members and type definitions
  */

  // typedef allows us to rename a standard data structure for added clarity
  typedef std::pair<int, Person>    Node;    // A Node has an ID (int) and a value (Person)
  typedef std::pair<Node *, Node *> Edge;    // An Edge is a connection between 2 Nodes

  // Store the list of edges (Connections)
  std::list<Edge> EdgeList;
  // Store the actual Nodes in our graph
  std::vector<Node> Graph;

  /*
    Private Utility Functions
  */

  // 1. Create a list of Edges
  // 2. Search the EdgeList object for edges where either node relates to the person
  // 3. Add them to the new list and return it
  std::list<Edge> _findEdges( Person person )
  {
    std::list<Edge> edges;

     for (Edge edge : EdgeList) {
        if (edge.first->second == person || edge.second->second == person) {
            // Add the edge to the list
            edges.push_back(edge);
        }
    }

    return edges;
  }

  // 1. Use a classic index-based loop on the Graph vector
  // 2. Find the Node where the value = the person provided
  // 3. When found, return the memory address
  // 4. If nothing is found, return nullptr
  Node * _nodeFor( Person person )
  {
        
    for (int i = 0; i < static_cast<int>(Graph.size()); i++)
    {
        
        if (Graph[i].second == person)
        {
            
            return &Graph[i];
        }
    }

    
    return nullptr;
  }

  // 1. Create a list of Node pointers to return
  // 2. Find the Node pointer for the person provided
  // 3. If the node is found (not nullptr) find the person's Edges
  // 4. Loop through the found edges and find the Node pointer that is NOT the person parameter
  // 5. Add the Node pointer to your return list and return that list at the end
  std::list<Node *> _friendsOf( Person person )
  {
    std::list<Node *> friends;

    Node * personNode = _nodeFor(person);

     if (personNode != nullptr)
    {
        // 4. Loop through the found edges and find the Node pointer that is NOT the person parameter
        for (auto & edge : EdgeList)
        {
            if (edge.first == personNode)
            {
                // Add the Node pointer to your return list
                friends.push_back(edge.second);
            }
            else if (edge.second == personNode)
            {
                // Add the Node pointer to your return list
                friends.push_back(edge.first);
            }
        }
    }

    // 5. Return the list of Node pointers
    return friends;

  }

  // This function will calculate the minimum # of edges between 2 Nodes
  // visited, min_edges and edge_count are passed by REFERENCE so they can be modified by this function
  // This function doesn't need to return anything since the important variables are references that are modified
  // Use the Node type's first property as the index for the visited vector.
  // You can use this example as a reference: https://www.geeksforgeeks.org/minimum-number-of-edges-between-two-vertices-of-a-graph-using-dfs/
  // Modifications need to be made for this to work with our structures but it can be done!
  void _minEdges( std::vector<bool> & visited, Person pStart, Person pEnd, int & min_edges, int & edge_count )
  {
        // Check if the starting and ending nodes are the same
    if (pStart == pEnd)
    {
        // Update the min_edges variable if necessary
        if (edge_count < min_edges)
        {
            min_edges = edge_count;
        }

        // Return since we found the minimum number of edges
        return;
    }

    // Find the Node for the starting Person
    Node * startNode = _nodeFor(pStart);

    // Mark the starting node as visited
    visited[startNode->first] = true;

    // Increment the edge count
    edge_count++;

    // Get the list of friends for the starting node
    std::list<Node *> friends = _friendsOf(pStart);

    // Iterate over the list of friends
    for (auto & minfriend : friends)
    {
        // Check if the friend has been visited
        if (!visited[minfriend->first])
        {
            // Call the function recursively with the friend as the starting node
            _minEdges(visited, minfriend->second, pEnd, min_edges, edge_count);
        }
    }

    // Decrement the edge count
    edge_count--;
 
  }



public:
  // Default Constructor
  SocialNetwork() = default;

  // Constructor with a {bracket, separated} list of Person objects
  SocialNetwork( const std::initializer_list<Person> & newUsers )
  {
    for( auto && person : newUsers )
    {
      Add( person );
    }
  }

  // Use the _minEdges() function to calculate the minimum # of edges between 2 people
  // 1. Create the visited vector; initialize it with the same size as the Graph vector so it has enough space!
  // 2. Create the min_edges int and initialize it to the size of the EdgeList (which would be the maximum)
  // 3. Create the edge_count int and set it to 0 to start.
  // 4. Call the _minEdges() function with the appropriate arguments
  // 5. Return the min_edges int. Our _minEdges() function will modify it as needed since it is passed via reference.
  int distanceBetween( Person p1, Person p2 )
  {
     // 1. Create the visited vector
    std::vector<bool> visited(Graph.size());

    // 2. Create the min_edges int and initialize it to the size of the EdgeList
    int min_edges = static_cast<int>(EdgeList.size());

    // 3. Create the edge_count int and set it to 0
    int edge_count = 0;

    // 4. Call the _minEdges() function with the appropriate arguments
    _minEdges(visited, p1, p2, min_edges, edge_count);

    // 5. Return the min_edges int
    return min_edges;

  }

  // List of people in the graph
  // 1. Just create a temporary list of people, loop through Graph and add each person to the temporary list.
  // 2. Return the temporary list
  std::list<Person> allPeople()
  {
        // 1. Create a temporary list of people
    std::list<Person> peopleList;

    // 2. Loop through Graph and add each person to the temporary list
    for (auto & node : Graph)
    {
        peopleList.push_back(node.second);
    }

    // 3. Return the temporary list
    return peopleList;

  }

  // Make use of Lambda functions to find people who meet certain criteria (First/Last Name and/or Age)
  // The first parameter is a lambda function. It returns bool & needs a noexcept flag after the parentheses when you go to use it.
  // Parameter 2 is optional and will help us decide how to sort the list after it is built.
  std::list<Person> findPeople( std::function<bool( Person )> filterFunction)
  {
    // 1. Create a list to return
    // 2. Loop through the Graph
    // 3. For each node in the Graph, run the Node's Person (value) through the filterFunction()
    // 4. IF the filterFunction() returns true, add that person to the output list
    // 5. Use the sortby variable to decide how to sort the list.
    // 6. std::list has a built-in sort function. It requires use of the comparator defined in the Person object.
    std::list<Person> outList;
    for( Node item : Graph )
    {
      if( filterFunction( item.second ) )
      {
        outList.push_back( item.second );
      }
    }
    return outList;
  }

  // Similar to findPeople but we only wnat to find 1 person
  // Use the findPeople function with a Lambda that checks for the provided first/last Names
  // The age is optional. If age < 0, ignore it. Otherwise use it in the filter & compare it to p.Age()
  // Make sure the resulting list of People is not Empty and, if not, select the first element using .front() and return it.
  // Return a list of people because you might NOT find the person, so the list will be empty!
  std::list<Person> findPerson( std::string firstName, std::string lastName, int age = -1 )
  {
  auto filter = [&](Person p) noexcept
   {
    bool ageCheck = age >= 0 ? ( p.Age() == age) : true;

    return p.FirstName() == firstName && p.LastName() == lastName && ageCheck;
   };
    

     return findPeople( filter );
  }

  // Use the _friendsOf() function to find everyone connected to the person
  // Output their information using std::cout (the Bio() function works well for this)
  void showFriends( Person person )
  {
    std::list<Node*> friends = _friendsOf(person);
    for(auto & friendNode :friends)
    {
      std::cout<< friendNode ->second.Bio() << std::endl;
    }
  
  }

  // Just a shortcut to be able to use showFriends() with first/last name and possibly age
  // Just use the findPerson() function and the showFriennds() function, this should only be 2 lines of code.
  void showFriends( std::string firstName, std::string lastName, int age = -1 )
  {
    std::list<Person> results = findPerson( firstName, lastName, age );
    if( results.empty() == false )
    {
      Person targetPerson = results.front();
      showFriends( targetPerson );
    }
    else{
      std::cout << "Person Not Found" << std::endl;
    }
    
  }

  // Add a Node to the graph
  // The first property should = Graph size and second = the Person to add
  void Add( Person newPerson )
  {
    // Create a new Node with the ID set to the current size of the Graph vector
    // and the value set to the newPerson parameter
    Node newNode{ static_cast<int>(Graph.size()), newPerson };
    // Add the new Node to the end of the Graph vector
    Graph.push_back(newNode);

  }

  // Create an Edge between 2 people
  // 1. Find each person's node using _nodeFor
  // 2. Create an edge using std::make_pair() and add it to the EdgeList
  void Connect( Person p1, Person p2 )
  {
    // Find the Node objects for each Person
    Node * node1 = _nodeFor(p1);
    Node * node2 = _nodeFor(p2);

    // Check if either Node was not found (returns nullptr)
    if (node1 == nullptr || node2 == nullptr)
    {
        // One or both of the Person objects were not found in the graph
        // Return without creating an Edge
        return;
    }

    // Create a new Edge object using std::make_pair() and the Node pointers
    Edge newEdge = std::make_pair(node1, node2);
    // Add the new Edge to the EdgeList
    EdgeList.push_back(newEdge);
  }

// Here you don't need +nodeFor you can just access the node on the graph with brackets
// Be sure to subtract 1 from these numbers because the graph vector index is 0-based!
// You'll need to use the address operator here...
  void Connect( int index1, int index2 )
  {
    index1 = index1-1;
    index2 = index2-1;
    // Check if either index is out of bounds
    if (index1 < 0 || index1 >= static_cast<int>(Graph.size()) || index2 < 0 || index2 >= static_cast<int>(Graph.size()))
    {
        // One or both of the indices are out of bounds
        // Return without creating an Edge
        return;
    }

    // Access the Node objects in the Graph vector with the provided indices
    Node * node1 = &Graph[index1];
    Node * node2 = &Graph[index2];

    // Create a new Edge object using std::make_pair() and the Node pointers
    Edge newEdge = std::make_pair(node1, node2);
    // Add the new Edge to the EdgeList
    EdgeList.push_back(newEdge);
  }

  // This does the exact same thing as connect but allows us to add many connections at once
  // You need a loop within a loop. The people parameter is itself a list of lists of Person objects.
  // Use a vector in loop 1 to store pairs of people found in loop 2
  // In loop 2, Use bracket[j] notation and the Connect() function to connect the 2 people found
  // Clear the vector before continuing the loop so it only ever has 2 people at once (vector has a .clear() function)
  void Connect( const std::initializer_list<std::initializer_list<Person>> & people )
  {
    for( auto i : people )
    {
      std::vector<Person> targets;
      for( auto j : i )
      {
        targets.push_back( j );
      }
      Connect( targets[0], targets[1] );
      targets.clear();
    }
  }

 // Just return the size of the Graph field. Graph is PRIVATE so your code in main.cpp
 // doesn't have any way of accessing it directly. 
  std::size_t graphSize()
  {
    
      return static_cast<int>(Graph.size());
  }
// Return the size of the EdgeList field. EdgeList is PRIVATE so your code in main.cpp
// doesn't have any way of accessing it directly. 
  std::size_t edgeCount()
  {
    return static_cast<int>(EdgeList.size());
  }
};
