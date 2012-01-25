// AMX profiler for SA-MP server: http://sa-mp.com
//
// Copyright (C) 2011-2012 Sergey Zolotarev
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include "call_graph.h"

namespace amx_profiler {

CallGraphNode::CallGraphNode(const std::shared_ptr<Function> &fn, 
                             const std::shared_ptr<CallGraphNode> &caller) 
	: fn_(fn)
	, caller_(caller)
	, std::enable_shared_from_this<CallGraphNode>()
{
}

void CallGraphNode::AddCallee(const std::shared_ptr<Function> &fn) {
	auto new_node = std::shared_ptr<CallGraphNode>(new CallGraphNode(fn, shared_from_this()));
	AddCallee(new_node);
}

void CallGraphNode::AddCallee(const std::shared_ptr<CallGraphNode> &node) {
	// Avoid duplicate functions
	for (auto iterator = callees_.begin(); iterator != callees_.end(); ++iterator) {
		if ((*iterator)->function()->address() == node->function()->address()) {
			return;
		}
	}
	callees_.push_back(node);
}

void CallGraphNode::Write(std::ostream &stream) const {
	if (!callees_.empty()) {
		std::string name;
		if (fn_) {
			name = fn_->name();
		} else {
			name = "Application";
		}
		for (auto iterator = callees_.begin(); iterator != callees_.end(); ++iterator) {			
			stream << '\t' << name << " -> " << (*iterator)->function()->name() << std::endl;
			(*iterator)->Write(stream);
		}
	}
}

CallGraph::CallGraph(const std::shared_ptr<CallGraphNode> &root)
	: root_(root)
	, sentinel_(new CallGraphNode(0))
{
	if (!root) {
		root_ = sentinel_;
	}
}

void CallGraph::Write(std::ostream &stream) const {
	stream << 
	"digraph prof {\n"
	"	size=\"6,4\"; ratio = fill;\n"
	"	node [style=filled];\n"
	;

	// Recursively write all nodes 
	sentinel_->Write(stream);

	stream <<
	"}\n";
}

} // namespace amx_profiler