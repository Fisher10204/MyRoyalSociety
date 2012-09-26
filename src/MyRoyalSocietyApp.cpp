//Jacob McCanty
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"
#include "cinder/Rand.h"


using namespace ci;
using namespace ci::app;
using namespace std;


//It may be a good idea to separate the classes and declarations into separtate .h and .cpp files

//Good variable names, makes it easy to understand
class RECTANGLE{
public:
	RECTANGLE(int colorRed, int colorGreen, int colorBlue, int xLoc, int yLoc, int heightGiven, int widthGiven);
	int red;
	int green;
	int blue;
	int x;
	int y;
	int width;
	int height;
};

RECTANGLE::RECTANGLE(int colorRed, int colorGreen, int colorBlue, int xLoc, int yLoc, int heightGiven, int widthGiven){
	red=colorRed;
	green=colorGreen;
	blue=colorBlue;
	x=xLoc;
	y=yLoc;
	width=widthGiven;
	height=heightGiven;
}
//staisfies requirement A
class Node{
public:
	Node();
	Node(RECTANGLE*);
	Node(bool);
	bool isEmpty();
	void insertAfter(Node*);
	void permanentlyDeleteNode(Node*);
	void remove(Node*);
	void removeWithoutDeleting(Node*);

	Node* returnEnd(Node*);
	void reverse(Node*);

	RECTANGLE* rect_;
	Node* next_;
	bool isSentinal;
};

Node::Node(){
	next_=this;
	isSentinal=true;
	rect_=new RECTANGLE(255,255,255, 50,50, 50,100);
}
Node::Node(RECTANGLE* rect_){
	this->rect_=rect_;
	next_=this;
	isSentinal=false;
}

//The boolean variable isFirst is slightly confusing

Node::Node(bool isFirst)
{
	next_=this;
	isSentinal=isFirst;
	rect_=new RECTANGLE(100,100,255, 75,75, 50,100);
}
//Technically this list will never be empty, but this will let us know if there are any other nodes
bool Node::isEmpty(){
	if(this->next_==this)
		return true;
	else
		return false;
}
//inserts this node after the whereAt node in the list
void Node::insertAfter(Node* whereAt)
{
	Node* temp=new Node;
	temp->rect_=this->rect_;
	temp->next_=whereAt->next_;
	whereAt->next_=temp;
}
//this deletes a node permanently
void Node::permanentlyDeleteNode(Node* node){
	delete(node);
}
//I will delete the node after removing it since nothing can point to it again
void Node::remove(Node* node){
	if(node->isSentinal)
		if(node->isEmpty())
			return;
	Node* prev_Node=node;
	while(prev_Node->next_!=node)
	{
		prev_Node=prev_Node->next_;
	}
	Node* temp=new Node;
	temp->next_=prev_Node->next_;
	prev_Node->next_=prev_Node->next_->next_;
	if(temp->next_->isSentinal)
		prev_Node->next_->isSentinal=true;
	permanentlyDeleteNode(temp->next_);
	permanentlyDeleteNode(temp);
}
//Since it may be helpful to not always delete nodes when removing them, this method does just that
void Node::removeWithoutDeleting(Node* node)
{
	Node* prev_Node=node;
	while(prev_Node->next_!=node)
	{
		prev_Node=prev_Node->next_;
	}
	prev_Node->next_=prev_Node->next_->next_;
}

//here the passed node should be the starting node, which should be the sentinal, even after reversing as the reverse method will update the first node in the list to be the sentinal
Node* Node::returnEnd(Node* nodeSentinel){
	Node* current=nodeSentinel;
	while(current->next_!=nodeSentinel)
	{
		current=current->next_;
	}
	return current;
}
//Satisfies requirement E
void Node::reverse(Node* the_Sentinel){
	if(the_Sentinel!=the_Sentinel->next_)
	{
		Node* temp=the_Sentinel->next_;
		removeWithoutDeleting(temp);
		reverse(the_Sentinel);
		temp->next_->next_=temp;
		temp->next_=the_Sentinel;
	}
	the_Sentinel->next_->isSentinal=true;
	the_Sentinel->isSentinal=false;
}

class MyRoyalSocietyApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void drawList(Node*, uint8_t* pixels);
	void prepareSettings(Settings* settings);
	void updatePositions(Node*);
  private:
	  Node* sentinel_;
	  Node* end_;
	  
	  int mousePositionX_;
	  int mousePositionY_;
	  bool left_Click_;
	  bool right_Click_;
	  int xSpeed;
	  int ySpeed;
	  uint8_t* pixels;

	  Surface* mySurface;
	  static const int windowWidth=800;
	  static const int windowHeight=600;
	  static const int surfaceSize=1024;
};

void MyRoyalSocietyApp::prepareSettings(Settings* settings)
{
	(*settings).setWindowSize(windowWidth,windowHeight);
	(*settings).setResizable(false);
}
// draws a linked list of nodes and their data, in this case rectangles
void MyRoyalSocietyApp::drawList(Node* theSentinel, uint8_t* pixels){
	Node* cur= theSentinel;
	do{
		int x=cur->rect_->x;
		int y=cur->rect_->y;
		int offset;
		for(int i=y; i<cur->rect_->height+cur->rect_->y; ++i)
		{
			for(int j=cur->rect_->x; j<cur->rect_->width+cur->rect_->x; ++j)
			{
				offset=3*(j+i*surfaceSize);
				pixels[offset]=cur->rect_->red;
				pixels[offset+1]=cur->rect_->green;
				pixels[offset+2]=cur->rect_->blue;
			}
		}
		cur=cur->next_;
	}while(cur!=theSentinel);
}

void MyRoyalSocietyApp::setup()
{
	sentinel_=new Node();
	end_=new Node(false);
	(*end_).insertAfter(sentinel_);

	int red = rand()%255;
	int green= rand()%255;
	int blue= rand()%255;
	Node* new_Node=new Node(new RECTANGLE(red,green,blue,rand()%40,rand()%70,rand()%50+10,rand()%75+10));
	(*new_Node).insertAfter(sentinel_);

	red=rand()%255;
	green=rand()%255;
	blue=rand()%255;
	new_Node=new Node(new RECTANGLE(red,green,blue,rand()%60,rand()%20,rand()%50+10,rand()%75+10));
	(*new_Node).insertAfter(sentinel_);

	red=rand()%255;
	green=rand()%255;
	blue=rand()%255;
	new_Node=new Node(new RECTANGLE(red,green,blue,rand()%100,rand()%120,rand()%50+10,rand()%75+10));
	(*new_Node).insertAfter(sentinel_);

	left_Click_=false;
	right_Click_=false;
	mySurface=new Surface(surfaceSize, surfaceSize, false);
	if(rand()%2==0){
		xSpeed=rand()%10+1;
		ySpeed=rand()%15+1;
	}
	else
	{
		xSpeed=(rand()%10)*(-1)-1;
		ySpeed=(rand()%15)*(-1)-1;
	}
	pixels=(*mySurface).getData();
}
//Satisfies requirement C, Note Doesn't work
void MyRoyalSocietyApp::mouseDown( MouseEvent event )
{
	mousePositionX_=event.getX();
	mousePositionY_=event.getY();
	right_Click_=event.isRightDown();
	left_Click_=event.isLeftDown();
}
//Satisfies requirement D
void MyRoyalSocietyApp::updatePositions(Node* the_sentinel)
{
	Node* cur=new Node;
	cur=the_sentinel;
	do{
		cur->rect_->x+=xSpeed;
		cur->rect_->y+=ySpeed;
		cur=cur->next_;
	}while(cur->next_!=the_sentinel);
}

void MyRoyalSocietyApp::update()
{
	//add new rectangle where mouse is and add it to the list
	if(left_Click_)
	{
		left_Click_=false;
		int red = rand()%255;
		int green= rand()%255;
		int blue= rand()%255;
		Node* new_Node=new Node(new RECTANGLE(red,green,blue,mousePositionX_,mousePositionY_,rand()%50+10,rand()%75+10));
		(*new_Node).insertAfter((*sentinel_).returnEnd(sentinel_));
	}
	//reverse; doesn't work, sentinel_ won't point to the right node (the first node) after 1 reverse
	if(right_Click_)
	{
		right_Click_=false;
		(*sentinel_).reverse(sentinel_);
	}
	updatePositions(sentinel_);
}

void MyRoyalSocietyApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	drawList(sentinel_, pixels);
	gl::draw(*mySurface);
}

CINDER_APP_BASIC( MyRoyalSocietyApp, RendererGl )
