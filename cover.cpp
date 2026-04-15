#include<bits/stdc++.h>
using namespace std;


const int DEPTH = 3;
const int ATTACK = 0;


typedef pair<int, int> pie;         //{parent, child}, parent < child
const int noNode = -1;


string NORMAL(int x, int t) {
    return "x_" + to_string(x) + "_t_" + to_string(t);
}
string EDGE(string u, string v) {
    return u + " + " + v + " >= 1;\n";
}
string BRANCHED(int x, int t, vector<int> &branches, int last = noNode) {
    string result = "x_" + to_string(x) + "_t_" + to_string(t);
    for(auto a : branches)
        result += "_e_" + to_string(a);
    if(last != noNode)
        result += "_e_" + to_string(last);
    return result;
}
string EXTRA(int depth, int t, vector<int> &branches, int last = noNode) {
    string result = "d_" + to_string(depth) + "_t_" + to_string(t);
    for(auto a : branches)
        result += "_e_" + to_string(a);
    if(last != noNode)
        result += "_e_" + to_string(last);
    return result;
}
string ATTACKED(int x, int t, vector<int> &branches, vector<bool> attacks) {
    string result = "x_" + to_string(x) + "_t_" + to_string(t);
    for(auto a : branches)
        result += "_e_" + to_string(a);
    for(int i=0; i<attacks.size(); i++)
        if(attacks[i])
            result += "_a_" + to_string(i);
    return result;
}
string PIN(int x, int t, vector<int> &branches, vector<bool> attacks, int index) {
    string result = "a_" + to_string(x) + "_" + to_string(index) + "_t_" + to_string(t);
    for(auto a : branches)
        result += "_e_" + to_string(a);
    for(int i=0; i<attacks.size(); i++)
        if(attacks[i])
            result += "_a_" + to_string(i);
    return result;
}
string BUF(int depth) {
    return string(2*depth, ' ');
}
struct node {
    int taken;
    int best;
    int parent;
    vector<int> children;
    node(int parent) {
        this->parent = parent;
    }
};

//debug only
void showNodes(vector<node> &nodes) {
    for(int i=0; i<nodes.size(); i++) {
        cout << "    Node: " << i << "\n";
        cout << "    taken: " << nodes[i].taken;
        cout << " best:" << nodes[i].best;
        cout << " parent: " << nodes[i].parent << "\n    children:";
        for(auto next : nodes[i].children)
            cout << " " << next;
        cout << "\n";
    }
}

vector<pie> input() {
    int n;          //number of edges (one less than number of vertices)
    cin >> n;
    vector<pie> edges(n);
    for(auto &p : edges)
        cin >> p.first >> p.second;         //assumes both in {0, ..., n}

    cout << "min: score;\n";
    //monotonicity of 'main tree'
    for(int v=0; v<=n; v++)
        cout << NORMAL(v, 0) << " >= 0;\n";
    for(int t=0; t<n; t++)
        for(int v=0; v<=n; v++)
            cout << NORMAL(v, t) << " <= " << NORMAL(v, t+1) << ";\n";
    if(DEPTH == 0 && ATTACK == 0)
        for(int v=0; v<=n; v++)
            cout << NORMAL(v, n) << " <= 1;\n";
    cout << "\n\n";

    return edges;
}
void recalculateNode(int cur, vector<node> &nodes, int attacked = 0) {
    //when the node is being attacked, it gets extra `attacked` leaves, that have `taken=1` and `best=0`
    nodes[cur].taken = 1;
    nodes[cur].best = attacked;
    for(auto &child : nodes[cur].children) {
        nodes[cur].taken += nodes[child].best;
        nodes[cur].best += nodes[child].taken;
    }
    nodes[cur].best = min(nodes[cur].best, nodes[cur].taken);
}
void recalculateAll(vector<node> &nodes, vector<bool> &attacks, int attacked) {
    for(int i = nodes.size()-1; i>=0; i--)
        recalculateNode(i, nodes, attacks[i] * attacked);
}
void updateVC(vector<node> &nodes) {
    int cur = nodes.size()-1;
    nodes[nodes[cur].parent].children.push_back(cur);
    while(cur != noNode) {
        recalculateNode(cur, nodes);
        cur = nodes[cur].parent;
    }
}
void nextAttack(vector<bool> &attacks) {            //basically "add one in binary"
    int index = 0;
    while(attacks[index] == true) {
        attacks[index] = false;
        index++;
    }
    attacks[index] = true;
}
void performAttacks(vector<node> &nodes, int turn, vector<int> branches) {
    vector<bool> attacks(nodes.size()+1, false);
    attacks[0] = true;          //skip the null-attack
    while(attacks.back() == false) {
        
        //monotonicity
        for(int j=0; j<=turn; j++)          //main nodes
            cout << BUF(DEPTH+1) << BRANCHED(j, turn, branches) << " <= " << ATTACKED(j, turn, branches, attacks) << ";\n";
        for(int d=1; d<=branches.size(); d++)         //branch nodes
            cout << BUF(DEPTH+1) << EXTRA(d, turn, branches) << " <= " << ATTACKED(d+turn, turn, branches, attacks) << ";\n";
        for(int j=0; j<nodes.size(); j++)           //end bound
            cout << BUF(DEPTH+1) << ATTACKED(j, turn, branches, attacks) << " <= 1;\n"; 
        cout << "\n";

        //pins
        for(int target=0; target<nodes.size(); target++) {
            if(!attacks[target])
                continue;
            for(int index=1; index<=ATTACK; index++) {
                //begin
                cout << BUF(DEPTH+2) << PIN(target, turn, branches, attacks, index) << " >= 0;\n";
                //end
                cout << BUF(DEPTH+2) << PIN(target, turn, branches, attacks, index) << " <= 1;\n";
                //edge
                cout << BUF(DEPTH+2) << EDGE(ATTACKED(target, turn, branches, attacks), PIN(target, turn, branches, attacks, index));
            }            
        }

        //vertex cover
        recalculateAll(nodes, attacks, ATTACK);
        cout << BUF(DEPTH+2) << ATTACKED(0, turn, branches, attacks);
        for(int j=1; j<nodes.size(); j++)           //nodes
            cout << " + " << ATTACKED(j, turn, branches, attacks);
        for(int target=0; target<nodes.size(); target++)            //pins
            if(attacks[target])
                for(int index = 1; index<=ATTACK; index++)
                    cout << " + " << PIN(target, turn, branches, attacks, index);
        cout << " <= " << nodes[0].best << "*score;\n\n";

        //go next
        nextAttack(attacks);
    }

    recalculateAll(nodes, attacks, 0);          //clear all changes
}
void extraPaths(vector<node> &nodes, int turn, vector<int> branches, int skip = noNode) {
    for(int i=0; i<nodes.size(); i++) {
        if(i == skip)
            continue;

        nodes.push_back(i);         //add extra
        updateVC(nodes);
        int depth = branches.size()+1;

        //monotonicity
        for(int j=0; j<=turn; j++)          //main nodes
            cout << BUF(depth) << BRANCHED(j, turn, branches) << " <= " << BRANCHED(j, turn, branches, i) << ";\n";
        for(int d=1; d<=branches.size(); d++)         //previously added branch nodes
            cout << BUF(depth) << EXTRA(d, turn, branches) << " <= " << EXTRA(d, turn, branches, i) << ";\n";
        branches.push_back(i);          //new branch node
        cout << BUF(depth) << EXTRA(branches.size(), turn, branches) << " >= 0;\n";

        //new edge
        if(i <= turn)           //connected to one of the main nodes
            cout << BUF(depth) << EDGE(BRANCHED(i, turn, branches), EXTRA(branches.size(), turn, branches));
        else                    //connected to an extra node
            cout << BUF(depth) << EDGE(EXTRA(i-turn, turn, branches), EXTRA(branches.size(), turn, branches));

        //vertex cover
        cout << BUF(depth);
        for(int j=0; j<=turn; j++)          //main nodes
            cout << BRANCHED(j, turn, branches) << " + ";
        for(int d=1; d<branches.size(); d++)        //branch nodes (all but last)
            cout << EXTRA(d, turn, branches) << " + ";
        cout << EXTRA(branches.size(), turn, branches) << " <= " << nodes[0].best << "*score;\n\n";
        
        //no more branches from here, need to add lower bounds
        if(depth == DEPTH) {
            if(ATTACK == 0) {
                for(int j=0; j<=turn; j++)          //main nodes
                    cout << BUF(depth+1) << BRANCHED(i, turn, branches) << " <= 1;\n";
                for(int d=1; d<=branches.size(); d++)         //branch nodes
                    cout << BUF(depth+1) << EXTRA(d, turn, branches) << " <= 1;\n";
                cout << "\n";
            }
            else
                performAttacks(nodes, turn, branches);
        }
        else 
            extraPaths(nodes, turn, branches);
            
        //come back from recursion - remove that added node
        branches.pop_back();
        nodes.pop_back();           //remove extra
        nodes[i].children.pop_back();
        int cur = i;
        while(cur != noNode) {
            recalculateNode(cur, nodes);
            cur = nodes[cur].parent;
        }
    }
}
void buildTree(vector<pie> &edges) {
    vector<node> nodes;
    nodes.emplace_back(noNode);

    int turn = 1;
    
    for(int i=0; i<edges.size(); i++) {
        auto [parent, cur] = edges[i];
        int next = (i<edges.size()-1) ? edges[i+1].first : noNode;
        nodes.emplace_back(parent);
        updateVC(nodes);
        
        cout << EDGE(NORMAL(parent, turn), NORMAL(cur, turn));  //EDGE(parent, cur, turn);

        cout << NORMAL(0, turn);
        for(int i=1; i<nodes.size(); i++)
            cout << " + " << NORMAL(i, turn);
        cout << " <= " << nodes[0].best << "*score;\n\n";
        
        //add extra paths here
        if(DEPTH > 0)
            extraPaths(nodes, turn, {}, next);
        else if(ATTACK > 0)
            performAttacks(nodes, turn, {});

        cout << "\n";

        turn++;
    }
}

int main() {            //assume everything we build is connected and input is normalised
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    vector<pie> edges = input();
    buildTree(edges);

    return 0;
}
