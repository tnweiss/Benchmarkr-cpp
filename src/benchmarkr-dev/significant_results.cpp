//
// Created by tyler on 2/7/22.
//

#include "benchmarkr-dev/significant_results.h"


benchmarkr::SignificantResults::SignificantResults(unsigned short max_size): _max_size(max_size), _size(0),
  _end(nullptr) {

}

unsigned int benchmarkr::SignificantResults::size() const {
  return _size;
}

bool benchmarkr::SignificantResults::add(benchmarkr::Result* result) {
  // create the current node
  auto* node_to_add = new SignificantNode(result);

  // if this is the first element, just set the end
  if (_end == nullptr) {
    _end = node_to_add;
    _size ++;
    return true;
  }

  // check to see if the result is more significant than the end
  if (node_to_add->abs_delta() < _end->abs_delta()) {
    if(_size < _max_size) {
      node_to_add->set_next(_end);
      _end = node_to_add;
      _size ++;
      return true;
    } else {
      return false;
    }
  }

  // placeholder for the next node to look at
  SignificantNode* next_node = _end;

  // while next node is less than insert value
  while(next_node->next() != nullptr && next_node->next()->abs_delta() < node_to_add->abs_delta()) {
    next_node = next_node->next();
  }

  node_to_add->set_next(next_node->next());
  next_node->set_next(node_to_add);

  // delete the end item
  if (_size >= _max_size) {
    SignificantNode* new_end = _end->next();
    delete _end;
    _end = new_end;
  } else {
    _size ++;
  }

  return true;
}

benchmarkr::SignificantResults::~SignificantResults() {
  SignificantNode* next_node;
  SignificantNode* node_to_delete = _end;

  while (node_to_delete != nullptr) {
    next_node = node_to_delete->next();

    delete node_to_delete;

    node_to_delete = next_node;
  }

}

std::vector<const benchmarkr::Result *> benchmarkr::SignificantResults::as_vector() const {
  std::vector<const Result*> ret_val;
  ret_val.reserve(_size);

  benchmarkr::SignificantResults::SignificantNode* curr = _end;
  while(curr != nullptr) {
    ret_val.push_back(curr->result());
    curr = curr->next();
  }

  std::reverse(ret_val.begin(), ret_val.end());
  return ret_val;
}

benchmarkr::SignificantResults::SignificantNode::SignificantNode(benchmarkr::Result *result): _result(result), _next(
    nullptr) {

}

benchmarkr::SignificantResults::SignificantNode *benchmarkr::SignificantResults::SignificantNode::next() const {
  return _next;
}

double benchmarkr::SignificantResults::SignificantNode::abs_delta() const {
  return std::abs(_result->delta());
}

const benchmarkr::Result *benchmarkr::SignificantResults::SignificantNode::result() const {
  return _result;
}

void benchmarkr::SignificantResults::SignificantNode::set_next(benchmarkr::SignificantResults::SignificantNode *after) {
  _next = after;
}
