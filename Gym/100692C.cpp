// I think it'll be equivalent to image segementation: english and french = foreground and background

#include <bits/stdc++.h>

using namespace std;

#include <bits/stdc++.h>

using namespace std;

const int V = 2 * 1000 + 10 * 200;
const int E = 2 * (V + V + V + 100 * 200);
const int N = V;
const int INF = INT32_MAX;

/*
 * edge list representation
 */

// the index of the first outgoing edge for each vertex, initialised to -1
int start[V];
// if e is an outgoing edge from u, succ[e] is another one, or -1
// cap[e] is the capacity/weight of the e
// to[e] is the destination vertex of e
int succ[E], cap[E], to[E];

int edge_counter = 0;
void add_single_edge(int u, int v, int c) {
    // set the properties of the new edge
    cap[edge_counter] = c, to[edge_counter] = v;
    // insert this edge at the start of u's linked list of edges
    succ[edge_counter] = start[u];
    start[u] = edge_counter;
    ++edge_counter;
}

void add_edge(int u, int v, int c) {
    add_single_edge(u, v, c); // original
    add_single_edge(v, u, 0); // residual edge

}

/*
 * dinics 1
 */

// assumes the residual graph is constructed as in the previous section
// n = #nodes, s = source, t = sink
int n, s, t;
// stores dist from s.
int lvl[V];
// stores first non-useless child.
int nextchld[V];

// constructs the BFS tree.
// Returns if the sink is still reachable.
bool bfs() {

    for (int i = 0; i < n; i++) lvl[i] = -1;
    queue<int> q;
    q.push(s); lvl[s] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        nextchld[u] = start[u]; // reset nextchld
        for (int e = start[u]; ~e; e = succ[e]) {
            if (cap[e] > 0) {
                int nxt = to[e];
                if (lvl[nxt] != -1) continue; // already seen
                lvl[nxt] = lvl[u] + 1;
                q.push(nxt);
            }
        }
    }
    return lvl[t] != -1;
}

/*
 * dinics 2
 */

// Finds an augmenting path with up to cflow flow.
int aug(int u, int cflow) {
    if (u == t) return cflow; // base case.
    // Note the reference here! We will keep decreasing nextchld[u]
    // Till we find a child that we can flow through.
    for (int &i = nextchld[u]; i >= 0; i = succ[i]) {
        if (cap[i] > 0) {
            int nxt = to[i];
            // Ignore edges not in the BFS tree.
            if (lvl[nxt] != lvl[u] + 1) continue;
            int rf = aug(nxt, min(cflow, cap[i]));
            // Found a child we can flow through!
            if (rf > 0) {
                cap[i] -= rf;
                cap[i^1] += rf;
                return rf;
            }
        }
    }
    lvl[u]=-1;
    return 0;
}

int mf() {
    int tot = 0;
    while (bfs())
        for (int x = aug(s,INF); x; x = aug(s,INF)) tot+=x;
    return tot;
}

#define MAXN 201

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int cases; cin >> cases;

    for(int cas = 0; cas < cases; ++cas) {
        fill(start, start + V, -1);
        fill(succ, succ + E, 0);
        fill(cap, cap + E, 0);
        fill(to, to + E, 0);
        edge_counter = 0;
        fill(lvl, lvl + V, 0);
        fill(nextchld, nextchld + V, 0);


        map<string, int> word_to_num;
        vector<int> e_sentence;
        vector<int> f_sentence;
        vector<int> sentences[MAXN];
        int total_words = 1;

        int n_sentences; cin >> n_sentences;
        string line;
        getline(cin, line);
        for(int i = 0; i < n_sentences; ++i) {
            getline(cin, line);
            istringstream ss(line);
            string word;

            while(ss >> word) {
                if(word_to_num.find(word) == word_to_num.end()) word_to_num.insert({word, total_words++});
                int word_num = word_to_num[word];

                if (i == 0) { // known english sentence
                  e_sentence.push_back(word_num);
                } else if (i == 1) { // known french sentence
                  f_sentence.push_back(word_num);
                } else { // sentence of unknown language
                  sentences[i - 2].push_back(word_num);
                }
            }
        }
        --total_words;
        s = 0;
        t = 1 + 2 * total_words;
        n = 2 + 2 * total_words;

        for(int i = 1; i <= total_words; ++i) {
          add_edge(i, i + total_words, 1);
        }
        for(const auto& word_num: e_sentence) {
          add_edge(s, word_num, INF);
        }

        for(const auto& word_num: f_sentence) {
          add_edge(word_num + total_words, t, INF);
        }

        for(int i = 0; i < n_sentences - 2; ++i) {
          vector<int>& cur_sentence = sentences[i];
          for(int u = 0; u < cur_sentence.size(); ++u) {
              for(int v = 0; v < cur_sentence.size(); ++v) {
                  if(u == v) continue;
                  add_edge(cur_sentence[u] + total_words, cur_sentence[v], INF);
              }
          }
        }

        cout << "Case #" << cas + 1 << ": " << mf() << "\n";
    }
}