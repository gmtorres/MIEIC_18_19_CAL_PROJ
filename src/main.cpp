
#include "graphviewer.h"
#include "ui.h"
#include "graph.h"


using namespace std;

template<class T>
GraphViewer * generateGraphViewer(Graph<T> *graph){

	vector<Vertex <T> *> v = graph->getVertexSet();

	if(v.size()==0)
		return nullptr;



	GraphViewer *gv = new GraphViewer(600, 600,!v[0]->hasPosition(), true);

	gv->createWindow(600, 600);

	gv->defineVertexColor(VERTEXNORMALCOLOR);
	gv->defineEdgeColor("black");

	for(unsigned int i = 0; i<v.size();i++){

		if(v[i]->hasPosition()){
			gv->addNode(v[i]->getInfo() , v[i]->getX() , v[i]->getY());
		}else
			gv->addNode(v[i]->getInfo());

		gv->setVertexLabel(v[i]->getInfo() , to_string(v[i]->getInfo()));
	}

	for(unsigned int i = 0; i<v.size();i++){
		vector<Edge<T>> t = v[i]->getEdges();
		for(unsigned int a = 0; a< t.size();a++){
			gv->addEdge(t[a].getEdgeId() , t[a].orig->getInfo() , t[a].dest->getInfo() ,  EdgeType::DIRECTED);
			gv->setEdgeLabel(t[a].getEdgeId(),to_string(t[a].getWeight()));
		}
	}

	gv->rearrange();
	return gv;
}

template<class T>
void setPickups(vector<T> v, GraphViewer * gv){
	for(unsigned int i = 0; i < v.size() ; i++)
		gv->setVertexLabel(v[i] , to_string(v[i]) + " P");
}
template<class T>
void setDeliveries(vector<T> v, GraphViewer * gv){
	for(unsigned int i = 0; i < v.size() ; i++)
		gv->setVertexLabel(v[i] , to_string(v[i]) + " D");
}

template<class T>
GraphViewer * generateOriginalGraphViewer(DeliverySystem<T> ds){
	GraphViewer * gv = generateGraphViewer(ds.getMap());
	setPickups(ds.getPickupPoints() , gv);
	setDeliveries(ds.getDeliverPoints() , gv);
	gv->rearrange();
	return gv;
}

template<class T>
GraphViewer* generateProcessedGraphViewer(DeliverySystem<T> ds) {
	GraphViewer* gv = generateGraphViewer(ds.getProcessedMap());
	setPickups(ds.getPickupPoints(), gv);
	setDeliveries(ds.getDeliverPoints() , gv);
	gv->rearrange();
	return gv;
}

template<class T>
void showPath( vector<Vertex<T> *> v , GraphViewer * gv){
	for(unsigned int i = 0; i < v.size();i++){
		gv->setVertexColor(v[i]->getInfo(),VERTEXPATHCOLOR);
		gv->rearrange();
#ifdef linux
		sleep(1000);
#else
		Sleep(1000);
#endif
	}
}

template<class T>
void showPath( vector<T> v , GraphViewer * gv){
	for(unsigned int i = 0; i < v.size();i++){
		cout<<v[i]<<endl;
		gv->setVertexColor(v[i],VERTEXPATHCOLOR);
		gv->rearrange();
#ifdef linux
		sleep(1000);
#else
		Sleep(1000);
#endif
	}
}

template<class T>
void showPath( vector<vector<T>> v , GraphViewer * gv){
	for(size_t i = 0; i < v.size() ; i++){
		vector<T> path = v[i];
		showPath(path , gv);
		for(unsigned int a = 0; a < path.size();a++){
			gv->setVertexColor(path[a],VERTEXNORMALCOLOR);
		}
		cout<<endl;
		gv->rearrange();
	}
}

template<class T>
void printPaths(vector<vector<T>> paths){
	for(unsigned int i = 0; i < paths.size() ; i++){
		for(unsigned int a = 0; a < paths[i].size();a++){
			cout<<paths[a][i]<<"->";
		}

		cout<<endl;
	}
}

void user_interface(){
	string specialty;
	  //float latitude, longitude;
	  vector<Vehicle<int>> vehicles;

	  Graph<int> graph = createGraph5();

	  DeliverySystem<int> ds(graph, 0);

	  while (true) {
	    print_main_menu();
	    int option = get_option();

	    switch (option) {
	    case 1: {
	    	GraphViewer *gv = generateOriginalGraphViewer(ds);
	    	ds.runEspecialidades();
	    	showPath(ds.getVehiclesCompletePath(),gv);
	    	break;
	    }
	    case 2: {
	    	Vehicle<int> vehicle;
	    	get_vehicle_info(vehicle);
	    	ds.addVehicle(vehicle);
	    	cout << endl << "»----------------------------------«" << endl << endl;
	    	break;
	    }
	    case 3: {
	    	Request<int> request = get_request();
	    	ds.addRequest(request);
	    	cout << endl << "»----------------------------------«" << endl << endl;
	    	break;
	    }
	    case 4: {
	    	cin.clear();
	    	cin.ignore(999999, '\n');
	    	string city = get_city();
	    	graph = readFromFile(city);
	    	break;
	    }
	    }
	  }
}

void tests(){

	Graph<int> graph = createGraph5();

	DeliverySystem<int> ds(graph , 0);

	ds.addVehicle("nenhuma");

	ds.addRequest(Request<int>(1 , 3 , "nenhuma"));
	ds.addRequest(Request<int>(4 , 2 , "nenhuma"));
	ds.addRequest(Request<int>(6 , 3 , "nenhuma"));

	GraphViewer *gv = generateOriginalGraphViewer(ds);

	ds.runEspecialidades();
	showPath(ds.getVehiclesCompletePath(),gv);

	getchar();
}

int main(int argc, char const *argv[]) {

	user_interface();
	//tests();

	return 0;
}
