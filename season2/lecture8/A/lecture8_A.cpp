#include <iostream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <memory>
#include <queue>
#include <algorithm>
#include <functional>

struct Node;
using NodePtr = std::shared_ptr<Node>;
struct Node {
  explicit Node(int id): id(id) { }
  std::vector<NodePtr> neighbs;
  int dist1 = 0;
  int dist2 = 0;
  NodePtr path1 = nullptr;
  NodePtr path2 = nullptr;
  int flag = 0;
  int id;
};
bool operator<(const NodePtr& l, const NodePtr& r) {
  return l->id < r->id;
}
using NodeMap = std::unordered_map<int, NodePtr>;

void postTraversal(NodePtr root, bool flag_init_state, std::function<void(NodePtr)> operation) {
  for(auto node: root->neighbs) {
    if(node->flag == flag_init_state) {
      node->flag = !flag_init_state;
      postTraversal(node, flag_init_state, operation);
    }
  }
  operation(root);
}

void preTraversal(NodePtr root, int flag_init_state, std::function<void(NodePtr)> operation) {
  operation(root);
  for(auto node: root->neighbs) {
    if(node->flag == flag_init_state) {
      node->flag = !flag_init_state;
      preTraversal(node, flag_init_state, operation);
    }
  }
}

int main() {
  int n;
  NodeMap nodes_map;
  std::cin >> n;
  if(n == 1) {
    std::cout << "0 1 1" << std::endl;
    return 0;
  }
  for(int i = 0; i < n - 1; ++i) {
    int a, b;
    std::cin >> a >> b;
    if(!nodes_map.count(a)) {
      nodes_map.insert({a, NodePtr(new Node(a))});
    }
    if(!nodes_map.count(b)) {
      nodes_map.insert({b, NodePtr(new Node(b))});
    }
    nodes_map[a]->neighbs.push_back(nodes_map[b]);
    nodes_map[b]->neighbs.push_back(nodes_map[a]);
  }

  NodePtr root = nodes_map.begin()->second;
  root->flag = 1;

  postTraversal(root, 0, [root](NodePtr node) {
    if(node->neighbs.size() == 1 && root->id != node->id) {
      node->dist1 = 0;
      node->dist2 = 0;
      return;
    }
    for(auto ch: node->neighbs) {
      if(ch->dist1 + 1 > node->dist1) {
        node->dist2 = node->dist1;
        node->path2 = node->path1;

        node->dist1 = ch->dist1 + 1;
        node->path1 = ch;
      }
      else if(ch->dist1 + 1 > node->dist2) {
        node->dist2 = ch->dist1 + 1;
        node->path2 = ch;
      }
    }
  });

  root->flag = 0;
  preTraversal(root, 1, [](NodePtr node){
    NodePtr parent = nullptr;
    for(auto neighb: node->neighbs) {
      if(neighb->flag == 0) {
        parent = neighb;
      }
    }
    if(!parent)
      return; // special case for root itself
    if(!parent->path2 || parent->path2->id != node->id) {
      if(parent->dist2 + 1 >= node->dist1) {
        node->dist2 = node->dist1;
        node->path2 = node->path1;

        node->dist1 = parent->dist2 + 1;
        node->path1 = parent;
      }
      else if(parent->dist2 + 1 >= node->dist2) {
        node->dist2 = parent->dist2 + 1;
        node->path2 = parent;
      }
    }

    if(!parent->path1 || parent->path1->id != node->id) {
      if(parent->dist1 + 1 >= node->dist1) {
        node->dist2 = node->dist1;
        node->path2 = node->path1;

        node->dist1 = parent->dist1 + 1;
        node->path1 = parent;
      }
      else if(parent->dist1 + 1 >= node->dist2) {
        node->dist2 = parent->dist1 + 1;
        node->path2 = parent;
      }
    }
  });

  std::vector<int> centers;
  int min_dist = INT32_MAX;
  for(auto& [id, node]: nodes_map) {
    if(node->dist1 < min_dist) {
      min_dist = node->dist1;
      centers.clear();
      centers.push_back(id);
    }
    else if(node->dist1 == min_dist) {
      centers.push_back(id);
    }
  }

  std::cout << min_dist << ' ' << centers.size() << ' ';
  std::sort(centers.begin(), centers.end());
  for(auto& center: centers) {
    std::cout << center << ' ';
  }
}

