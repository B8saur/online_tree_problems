#include<bits/stdc++.h>
using namespace std;

#define VAR1(x, t) "x_" << x << "_t_" << t
#define VARE(x, t, e) "x_" << x << "_t_" << t << "_e_" << e
#define EXTRA(f, t) "f_" << f << "_t_" << t
#define EDGE(u, v, t) "x_" << u << "_t_" << t << " + " << "x_" << v << "_t_" << t << " <= 1;\n"
// #define DEPTH 1
typedef pair<int, int> pie;         //{parent, child}, parent < child
const int noParent = -1;

struct node {
    int notTaken;
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
        cout << "    notTaken: " << nodes[i].notTaken;
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

    //old output
    cout << "max: score;\n";
    //monotonicity of 'main tree'
    for(int v=0; v<=n; v++)
        cout << VAR1(v, 0) << " <= 1;\n";
    for(int t=0; t<n; t++)
        for(int v=0; v<=n; v++)
            cout << VAR1(v, t) << " >= " << VAR1(v, t+1) << ";\n";
    for(int v=0; v<=n; v++)
        cout << VAR1(v, n) << " >= 0;\n";
    cout << "\n\n";

    return edges;
}
void recalculateNode(int cur, vector<node> &nodes) {
    nodes[cur].notTaken = 0;
    nodes[cur].best = 1;
    for(auto &child : nodes[cur].children) {
        nodes[cur].notTaken += nodes[child].best;
        nodes[cur].best += nodes[child].notTaken;
    }
    nodes[cur].best = max(nodes[cur].best, nodes[cur].notTaken);
}
void updateIS(vector<node> &nodes) {
    int cur = nodes.size()-1;
    nodes[nodes[cur].parent].children.push_back(cur);
    while(cur != noParent) {
        recalculateNode(cur, nodes);
        cur = nodes[cur].parent;
    }
}
void extraPaths(vector<node> &nodes, int turn) {
    turn++;
    for(int i=0; i<nodes.size(); i++) {
        nodes.push_back(i);         //add extra
        updateIS(nodes);

        //main nodes in new timeline
        for(int j=0; j<nodes.size()-1; j++) {
            //this is already upper limit for the node in new timeline, no need to limit by 1
            cout << VAR1(j, turn-1) << " >= " << VARE(j, turn, i) << ";\n";
            cout << VARE(j, turn, i) << " >= 0;\n";
        }
        //new temporary node
        cout << EXTRA(i, turn) << " <= 1;\n";
        cout << EXTRA(i, turn) << " >= 0;\n";
        //edge from it
        cout << VARE(i, turn, i) << " + " << EXTRA(i, turn) << " <= 1;\n";
        //independent set
        for(int j=0; j<nodes.size()-1; j++)
            cout << VARE(j, turn, i) << " + ";
        cout << EXTRA(i, turn) << " >= " << nodes[0].best << "*score;\n";
        cout << "\n";

        nodes.pop_back();           //remove extra
        nodes[i].children.pop_back();
        int cur = i;
        while(cur != noParent) {
            recalculateNode(cur, nodes);
            cur = nodes[cur].parent;
        }
    }
}
void buildTree(vector<pie> &edges) {
    vector<node> nodes;
    nodes.emplace_back(noParent);

    int turn = 1;
    for(auto [parent, cur] : edges) {
        nodes.emplace_back(parent);
        updateIS(nodes);
        
        cout << "  " << EDGE(parent, cur, turn);

        cout << "  " << VAR1(0, turn);
        for(int i=1; i<nodes.size(); i++)
            cout << " + " << VAR1(i, turn);
        cout << " >= " << nodes[0].best << "*score;\n\n";
        
        //TODO: add extra paths here
        extraPaths(nodes, turn);
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
