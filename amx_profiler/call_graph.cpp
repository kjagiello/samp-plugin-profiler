// Copyright (c) 2011-2013, Zeex
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "call_graph.h"
#include "function.h"
#include "function_statistics.h"

namespace amx_profiler {

CallGraph::CallGraph(CallGraphNode *root)
	: root_(root)
	, sentinel_(new CallGraphNode(this, 0))
{
	if (!root) {
		root_ = sentinel_;
	}
}

CallGraph::~CallGraph() {
	delete sentinel_;
	for (NodeSet::const_iterator iterator = nodes_.begin();
			iterator != nodes_.end(); ++iterator) {
		delete *iterator;
	}
}

void CallGraph::OwnNode(CallGraphNode *node) {
	nodes_.insert(node);
}

void CallGraph::Traverse(Visitor *visitor) const {
	sentinel_->Traverse(visitor);
}

bool CallGraphNode::Compare::operator()(const CallGraphNode *n1, const CallGraphNode *n2) const {
	return n1->stats()->function()->address() < n2->stats()->function()->address();
}

CallGraphNode::CallGraphNode(CallGraph *graph, FunctionStatistics *stats, CallGraphNode *caller) 
	: graph_(graph)
	, stats_(stats)
	, caller_(caller)
{
}

CallGraphNode *CallGraphNode::AddCallee(FunctionStatistics *stats) {
	CallGraphNode *node = new CallGraphNode(graph_, stats, this);
	return AddCallee(node);
}

CallGraphNode *CallGraphNode::AddCallee(CallGraphNode *node) {
	graph_->OwnNode(node);
	callees_.insert(node);
	return node;
}

void CallGraphNode::Traverse(CallGraph::Visitor *visitor) const {
	visitor->Visit(this);
	for (CalleeSet::const_iterator iterator = callees_.begin();
			iterator != callees_.end(); ++iterator) {
		(*iterator)->Traverse(visitor);
	}
}

} // namespace amx_profiler
