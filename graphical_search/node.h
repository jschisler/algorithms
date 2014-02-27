#include <vector>
#include <functional>
#include <set>
#include <queue>
#include <map>
#include <iterator>
#include <stack>
#include <list>
#include <complex>

struct node { 
	int value; 
	std::vector <node*> neighbors; 
};

/*
	With breadth first search, abbreviated bfs, we study our first graph 
	search algorithm. bfs starts from an origin node and visits all reachable 
	nodes in a graph in order of their distance from the origin. The number of 
	edges from the origin measures distance. bfs is characterized by the use of 
	a queue to maintain the order in which nodes are visiting. The queue contains 
	the known frontier of the search, which are all nodes that are known to 
	exist but have not yet been visited.

	As mentioned above, graphs can have cycles between the nodes. To avoid 
	doubling back while traversing the graph bfs must keep track of all visited 
	nodes and not add nodes multiple times to the queue. To do this, we use a 
	set to track our history.

	As with tree traversal, bfs is usually required to do more than simply pop 
	every node from the queue. Often bfs is implemented to take a general 
	visit function that is called each time a node is examined. We use the 
	following definition for a visit function.
*/
typedef std::function<void(node*)> Visitor;

void bfs(node* origin, Visitor visit) {
	std::set<node*> queued; 
	std::queue<node*> queue; 
	
	queue.push(origin); 
	queued.insert(origin); 
	
	while (!queue.empty()) {
		node* current = queue.front();

		queue.pop(); 
		visit(current); 
		
		for (auto neighbor : current->neighbors) { 
			if (queued.end() == queued.find(neighbor)) { 
				queue.push(neighbor); 
				queued.insert(neighbor); 
			} 
		}
	}
}

/*
	Since bfs visits each node in order of its distance from the origin, it can 
	be used to solve the problem of finding the minimal distance from an origin 
	to all other nodes in a graph. In doing so we implement a special case of 
	Dijkstra’s algorithm for the case where all edges are of equal weight.

	The idea is to update the known distance of a node when we first add it to 
	the queue. Doing so will require keeping track of the state of node 
	exploration. The functional get_distance sets distance of unvisited nodes 
	to infinity. When a node is first encountered, all the children are given 
	distance equal to the distance of the parent plus one. Since bfs visits 
	each node in order of its distance, a parent’s distance will always be set 
	before a child is encountered. 
	
	Translating this into code, we have the following listing:
*/
void find_distances(node* origin, std::map <node*, size_t>* distances) {
	auto distance = [distances](node* vertex) { 
		auto distance_itr = distances->find(vertex); 
		
		if (distances->end() == distance_itr)
			return std::numeric_limits < size_t >::max();
		
		return distance_itr->second;
	}; 
	
	auto visit = [distances, distance](node* vertex) { 
		if (distances->empty()) 
			(*distances)[vertex] = 0; 
		
		for (node* neighbor : vertex->neighbors) { 
			auto vertexDistance = distance(vertex); 
			auto neighborDistance = distance(neighbor); 
			
			(*distances)[neighbor] = std::min(neighborDistance, 1 + vertexDistance);
		}
	};
	
	bfs(origin, visit);
}

/*
	My favorite interview question asks a candidate to determine if two people 
	are related. In this question, each person is a node. Every node has at 
	most two parents. The question is framed as having incomplete genealogical 
	data, which is the full ancestry of each individual may not be known. This 
	gives rise to graphs that may not be connected.

	To solve this problem, we first represent the domain as a directed graph. 
	Each person is a node, with the key being an identifier for a person. Every 
	node is adjacent to at most two other nodes representing the parents. Edges 
	are orientated from a child to parent. The following listing creates a graph 
	from the parental data . The nodes of the graph are represented in the map 
	specialization GeneologicalData. The neighbors of each node are the values 
	associated with the entry in the map.
*/
typedef unsigned Person;
typedef std::vector<Person> Parents; 
typedef std::map<Person, Parents> GeneologicalData; 
typedef std::set<Person> Ancestry;

void find_ancestry(const GeneologicalData& data, const Person& id, Ancestry* ancestry) { 
	std::queue<Person> queue; queue.push(id); 
	
	ancestry->insert(id); 
	
	while (queue.size()) { 
		auto entry = data.find(queue.front()); 
		
		queue.pop(); 
		
		if (entry == data.end()) 
			continue; 
		
		for (auto parent : entry->second) { 
			if (ancestry->end() == ancestry->find(parent)) { 
				ancestry->insert(parent); queue.push(parent); 
			} 
		} 
	} 
}

/*
	If the intersection of these sets is non-empty, then we are assured that 
	the individuals are related. Putting this all together, the solution to 
	the problem is given below.
*/
bool related(const GeneologicalData& data, Person id1, Person id2) { 
	Ancestry ancestry1; 
	find_ancestry(data, id1, &ancestry1); 
	
	Ancestry ancestry2; 
	find_ancestry(data, id2, &ancestry2); 
	
	Ancestry intersection; 
	std::set_intersection(ancestry1.begin(), ancestry1.end(), ancestry2.begin(), ancestry2.end(), std::inserter(intersection, intersection.begin())); 
	
	return !intersection.empty(); 
}

/*
	Depth first search is a graph traversal algorithm characterized by its use 
	of a stack to maintain the frontier of traversal. We often abbreviate it 
	as dfs. There are many important differences with bfs. Depth first search 
	attempts to discover the next longest path, while breadth first search 
	iterates through a frontier of equidistant nodes . Further, for each vertex 
	visited in depth first search the stack maintains a path from that node to 
	the root of the search. In bfs, the order of traversal history is lost when 
	the next node is visited.

	As with bfs, an implementation of dfs requires maintaining a list of vertices 
	that have been visited. There are implicitly two such lists. The nodes on the 
	stack will have their children visited. The nodes already visited will never 
	again be elements of the stack. The stack contains pairs of iterators, one 
	for a node and for that nodes adjacency list. The iterator maintains the 
	order in which neighbors are to be visited.
*/
void dfs(node* origin, Visitor visit) { 
	std::set<node*> queued; 
	std::stack<std::pair<node*, std::vector<node*>::iterator>> stack; 
	
	stack.push({ origin, origin->neighbors.begin() }); 
	queued.insert(origin); 
	
	while (!stack.empty()) { 
		auto current = stack.top().first; 
		auto& neighbor = stack.top().second; 
		
		if (neighbor != current->neighbors.end()) { 
			node* next = * neighbor; 
			++neighbor; 
			
			if (queued.end() == queued.find(next)) { 
				stack.push({ next, next->neighbors.begin() }); 
				queued.insert(next); 
			} 
			
			continue; 
		} 
		visit(current); 
		stack.pop(); 
	} 
}

/*
	Given a graphical representation of sub-processes of a large task, where 
	each node in the graph represents a sub-process. The children of a node 
	depend on the completion of their parent nodes. The problem is to determine 
	an execution order for all the processes in which no process is started 
	before its parents have completed. For this problem the directed graph is 
	special in that it has no cycles, and is called a directed acyclic graph. 
	If it had cycles then no sequential execution order would exist.

	This problem of determining sequential execution order can be solved by dfs. 
	Suppose the origin is a node that has no incoming edges. If we begin dfs 
	from this node, because there are no cycles we will eventually reach a node 
	that has no children. This node represents a process that is dependent upon 
	the entire contents of the stack during dfs. We save this to the back of a 
	list holding the reverse of execution order.

	We continue in this fashion until every node is visited and added to the 
	list. If the graph has not been fully traversed but dfs has an empty stack, 
	we choose the next node from the graph with no incoming edges and iterate.

	When this process completes, the list contains the reverse of the execution 
	order. Reversing the list, we then have execution order.

	The following listing implements the determination of execution order of the 
	graph starting with origin. In this listing we assume there is exactly one 
	node with no incoming edges, and that node is provided as the parameter. 
	With an implementation of dfs that takes a Visitor parameter, the code is 
	simplified by using a functional.
*/
void topological_sort(node* origin, std::list<node*>* list) { 
	auto visitor = [list](node* vertex) { 
		list->push_back(vertex);
	}; 
	
	dfs(origin, visitor); 
	list->reverse(); 
}

/*
	Suppose we are given a graph of nodes represented as an adjacency list, the 
	question is to determine if there exist any cycles in the graph. Depth first 
	search visits nodes by following edges. An edge that is traversed is called 
	a tree edge. When dfs attempts to visit an element that is already on the 
	stack, that edge is called a back edge. Such an edge provides positive 
	identification of a cycle, as it loops back to a path already traversed. So 
	the problem of determining if a graph contains a cycle is as simple as 
	finding a single back-edge in a dfs.

	Finding a cycle is simplified if the Visitor is provided the stack when 
	called, since it is a simple check to see if the visited node has back edges. 
	However, this is not necessary. Consider maintaining a list of visited 
	nodes in Visitor. Notice that in our implementation, dfs visits a node only 
	after visiting all of its children except in the case that one of its 
	children is on the stack. So if any node provided to Visitor has any 
	children that have not already been visited then a cycle exists in the 
	graph. We have detected a back edge. With this intuition, we can quickly 
	develop a solution.
*/
bool detect_cycle(node* origin) { 
	bool cycle_detected = false; 
	std::set<node*> visited;
	auto visitor = [&cycle_detected, &visited](node* vertex) { 
		visited.insert(vertex); 
		
		for (auto neighbor : vertex->neighbors) { 
			if (visited.end() == visited.find(neighbor)) { 
				cycle_detected = true; 
			} 
		} 
	};
	
	dfs(origin, visitor); 
	
	return cycle_detected; 
}

typedef std::vector<std::vector<char>> GameBoard; 
typedef std::pair<size_t, size_t> Position;

//  FIXME:  implement!
bool isword(const std::string&) { return false; }

void boggle(const GameBoard& board, const Position& pos, std::set<std::string>* words) { 
	static const std::vector<int> offsets = { -1, 0, 1 }; 
	std::set<std::pair<size_t, size_t>> visited; 
	std::string word; 
	std::function<void(Position)> boggle_dfs = [&](Position pos) { 
		for (auto row_offset : offsets) { 
			for (auto col_offset : offsets) { 
				Position next_pos = { pos.first + row_offset, pos.second + col_offset }; 
				
				auto element = visited.find(next_pos); 
				
				if (next_pos.first < board.size() && next_pos.second < board[next_pos.first].size() && visited.end() == element) { 
					visited.insert(next_pos); 
					word += board[next_pos.first][next_pos.second]; 
					
					if (isword(word)) { 
						words->insert(word); 
					} 
					
					boggle_dfs(next_pos); 
					visited.erase(visited.find(next_pos)); 
					word.resize(word.length() - 1); 
				} 
			} 
		} 
	}; 
	
	boggle_dfs(pos); 
}

void boggle(const GameBoard& board, std::set<std::string>* words) { 
	for (size_t row = 0; row < board.size(); ++row) { 
		for (size_t col = 0; col < board[row].size(); ++col) { 
			boggle(board, { row, col }, words); 
		} 
	} 
}

/*
	A* Search 
	
	We conclude graphical search with a brief example of A* search. A* search 
	is a graphical search which uses a priority queue to order the frontier of 
	exploration. The value of a node in the priority queue can be given by a 
	heuristic measurement of the distance to a goal. In this way it differs 
	from both bfs and dfs in that the order in which the nodes are to be visited 
	in not fixed by the order in which they are discovered.

	Suppose we are given a maze represented by a square n × n matrix defined as 
	a GameBoard. We are given a start position and an end position. The 
	question is to write a function that produces a path through the maze.

	To begin we refer to each position in the maze as coordinates, and the 
	function passable determines whether or not that node is passable.
*/
//  FIXME:  implement!
bool passable(const GameBoard& board, const Position& pos) { return false; }

/*
	Only positions above, below, left, and right are neighbors. Neighbors that 
	are passable are adjacent to a position in the board.

	Now to solve the problem efficiently with A* search, we would like to visit 
	each node in order of its distance from the exit of the maze. The hope is 
	that positions nearer the exit will have shorter paths to the exit.

	However, if we knew the actual distance we could solve the problem by 
	traversing from the exit to the start. Instead we use the heuristic that 
	we want to visit the next node that is closest to the exit of the maze where 
	distance is measured as if there was always a path directly from the node 
	to the exit. So to apply A * search , we will define a function which 
	calculates the shortest possible distance between a position in the maze 
	and the exit. While many metrics are available, we use Manhattan distance 
	as our maze distance.
*/
size_t distance(const std::pair<size_t, size_t> begin, const std::pair<size_t, size_t> end) { 
	return std::abs((int)(begin.first - end.first)) + std::abs((int)(begin.second - end.second));
}

/*
	In solving the maze, we being at the start and continue as with dfs by 
	iteratively constructing a path. However we use a priority queue instead 
	of stack, where the distance heuristic gives the priority of a node. Nodes 
	are considered in lowest priority value first. When the path reaches the 
	exit, the path through the maze is reconstructed and the function 
	terminates. If no solution exists, we return an empty path.
*/
//  There's a compile error here for some reason
#if 0
void maze(const GameBoard& maze, const Position& start, const Position& exit, std::list<Position>* route) {
	static const std::vector<std::pair<size_t, size_t>> offsets = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } }; 
	std::map<Position, Position> parent; 
	std::priority_queue<std::pair<size_t, Position>, 
						std::vector<std::pair<size_t, Position>>, 
						std::greater<std::pair<size_t, Position>>> priority_queue; 
	
	priority_queue.push({ distance(start, exit), start }); 
	parent[start] = start; 
	
	while (!priority_queue.empty()) {
		auto current = priority_queue.top().second; 
		
		if (exit == current) { 
			do { 
				route->push_front(current); 
				current = parent[current]; 
			} while (current != start); 
			
			route->push_front(start); 
			return; 
		} 
		
		priority_queue.pop();

		for (auto offset : offsets) {
			auto neighbor = Position(current.first + offset.first, current.second + offset.second); 
			
			if (neighbor.first < maze.size() && neighbor.second < maze[neighbor.first].size() && parent.end() == parent.find(neighbor) && passable(neighbor)) { 
				parent[neighbor] = current; 
				priority_queue.push({ distance(neighbor, exit), neighbor });
			}
		}
	}
}
#endif