#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

struct Dot { float x, y; };

struct Box {
    float x, y, size;
    // makes  a box with a size
    
    bool contains(Dot d) {
        //checks if its inside
        return (d.x >= x && d.x <= x + size && d.y >= y && d.y <= y + size);
    }
};

class QuadTree {
    Box box;
    vector<Dot> dots;
    bool divided = false;
    QuadTree *nw, *ne, *sw, *se; 
    // takes it and checks if its chopped up alr . then it makes placeholders become smaller boxes

public:
    QuadTree(Box b) { 
        // runs when the first tree is made then it sets the placeholder with nullprtr
        box = b; 
        nw = nullptr;
        ne = nullptr;
        sw = nullptr;
        se = nullptr;
    }
    
    ~QuadTree() { 
        delete nw; 
        delete ne; 
        delete sw; 
        delete se; 
        // deletes it after
    }

    void addDot(Dot d) {
        if (!box.contains(d)) {
            // ifg the dot isnt in the box it kicks it out and stops
            return; 
        }

        if (dots.size() < 4 && !divided) {
            dots.push_back(d);
            return;
            // checks if box has room max of 4 if there is a space it adds one into it
        }

        if (!divided) {
            // if its not divided it splits the box into 4 seperate by spiliing it then sets divided to true. then it shows that its divided so its only 1 deep
            float s = box.size / 2.0f; 
            nw = new QuadTree({box.x, box.y + s, s});
            ne = new QuadTree({box.x + s, box.y + s, s});
            sw = new QuadTree({box.x, box.y, s});
            se = new QuadTree({box.x + s, box.y, s});
            divided = true;
        }
        // adds a dot to each box only the right box will accept it
        nw->addDot(d); 
        ne->addDot(d); 
        sw->addDot(d); 
        se->addDot(d);
    }
};

int main() {
    int totalDots = 10000;
    // makes the total points to run/ runs 10000 and creates a  new dot after
    cout << "Test W # " << totalDots << " points" << endl;
    

    vector<Dot> allDots;

    for (int i = 0; i < totalDots; i++) {
        Dot newDot;
        newDot.x = (float)(rand() % 1000);
        newDot.y = (float)(rand() % 1000);
        allDots.push_back(newDot);
    }

    //------------------------------------------------------------------------------------------------
    // this is the slower check with nested loops we start tyhe stop watct with t1 and end it with t2
    auto t1 = chrono::high_resolution_clock::now();
    int dummy_collisions = 0;
    for (int i = 0; i < totalDots; i++) {
        for (int j = i + 1; j < totalDots; j++) { 
            if (allDots[i].x == allDots[j].x && allDots[i].y == allDots[j].y) {
                dummy_collisions++; 
            }
        }
    }
    auto t2 = chrono::high_resolution_clock::now();
    cout << "Brute Force Time: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "  ms\n";
    // this shows the time it took
    //------------------------------------------------------------------------------------------------


    //------------------------------------------------------------------------------------------------
    auto t3 = chrono::high_resolution_clock::now();
    // starts the stopwatch for the quciker quadtree
    Box initialBox;
    initialBox.x = 0;
    initialBox.y = 0;
    initialBox.size = 1000;
    QuadTree tree(initialBox); 
    // this goies through  and we throw each dot into throw dot into the tree
    for (int i = 0; i < allDots.size(); i++) {
        tree.addDot(allDots[i]); 
    }
    
    auto t4 = chrono::high_resolution_clock::now();
    cout << "QuadTree Time: " << chrono::duration_cast<chrono::milliseconds>(t4 - t3).count() << " ms\n";
    //------------------------------------------------------------------------------------------------
    return 0;
}