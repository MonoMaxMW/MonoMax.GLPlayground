﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MonoMax.CPQ.Models.Nodes
{
    public sealed class RootNode : Node
    {
        public RootNode()
        {
            _childNodes.Add(new TextNode());
            _childNodes.Add(new TextNode());
            _childNodes.Add(new TextNode());
            _childNodes.Add(new TextNode());

        }
    }
}
