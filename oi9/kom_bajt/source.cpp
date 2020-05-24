#include <iostream>
#include <vector>
#include <cmath>
using namespace std;


class Graph {
    vector<vector<uint>> graph_data;

    void addEdge(int a, int b) {
        graph_data[a].push_back(b);
        graph_data[b].push_back(a);
    }
public:
    Graph(size_t graph_size) {
        graph_data = vector<vector<uint>>(graph_size + 1, vector<uint>(0));
    }

    void readEdges() {
        for(size_t i = 0; i < (graph_data.size() - 2); ++i) {
            int a, b;
            cin >> a >> b;
            addEdge(a, b);
        }
    }

    auto &getRawData() {
        return graph_data;
    }

    void printGraphData() {
        for (int v = 1; v < graph_data.size(); ++v) {
            cout << "--> " << v << endl;
            cout << "\t";
            for(auto u : graph_data[v])
                cout << "-> " << u << ";";
            cout << endl;
        }
    }
};

class LCA {
    vector<bool> visited;
    vector<uint> tree_size;
    vector<uint> pre_index;
    vector<uint> node_depth;
    vector<vector<uint>> parent;
    vector<vector<uint>> &graph_data;

    int tree_depth;
    int pre_counter;

    void dfs(int v) {
        pre_counter += 1;
        pre_index[v] = pre_counter;

        for (auto u : graph_data[v]) {
            if (!visited[u]) {
                visited[u] = true;
                parent[u][0] = v;
                node_depth[u] = node_depth[v] + 1;
                dfs(u);
            }
        }

        tree_size[v] = pre_counter + 1 - pre_index[v];
    }
public:
    LCA(vector<vector<uint>> &prov_graph_data) : graph_data(prov_graph_data) {
        pre_counter = 0;
        tree_depth = log2(graph_data.size()) + 1;
        pre_index = vector<uint>(graph_data.size(), 0);
        tree_size = vector<uint>(graph_data.size(), 0);
        node_depth = vector<uint>(graph_data.size(), 0);
        visited = vector<bool>(graph_data.size(), false);
        parent = vector<vector<uint>>(graph_data.size(), vector<uint>(tree_depth, 0));

        visited[1] = true;
        dfs(1);

        parent[1][0] = 1;
        for (int l = 1; l < tree_depth; ++l)
            for(int u = 1; u < graph_data.size(); ++u)
                parent[u][l] = parent[parent[u][l - 1]][l - 1];
    }

    ~LCA() {}

    uint get_lca(int a, int b) {
        if (is_child(a, b)) return b;
        if (is_child(b, a)) return a;

        for(int j = tree_depth - 1; j >= 0;) {
            if (is_child(b, parent[a][j]))
                --j;
            else
                a = parent[a][j];
        }

        return parent[a][0];
    }

    bool is_child(int f, int c) {
        return (pre_index[f] >= pre_index[c]) && (pre_index[f] < (pre_index[c] + tree_size[c]));
    }

    uint get_depth(int v) {
        return node_depth[v];
    }
};

int main() {
    ios_base::sync_with_stdio(NULL);
    cout.tie(NULL);
    cin.tie(NULL);

    int N;
    cin >> N;
    Graph graph(N);
    graph.readEdges();
    // graph.printGraphData();

    int d_path = 0;
    int c_v = 1;
    LCA lca(graph.getRawData());

    int M;
    cin >> M;
    for(int i = 0; i < M; ++i) {
        int v;
        cin >> v;

        // cout << "V: " << v << endl;
        // cout << "\tNODE_DEPTH: " << lca.get_depth(v) << endl;
        // cout << "\tC_V: " << c_v << " DEPTH: " << lca.get_depth(c_v) << endl;

        auto n_lca = lca.get_lca(c_v, v);
        // cout << "\tLCA: " << n_lca << endl;
        auto distance = (int)(lca.get_depth(c_v) + lca.get_depth(v)) - (2 * (int)lca.get_depth(n_lca));
        // cout << "\tDISTANCE: " << distance << endl;

        c_v = v;
        d_path += distance;
    }

    // cout << "D_PATH: ";
    cout << d_path << endl;
    return 0;
}