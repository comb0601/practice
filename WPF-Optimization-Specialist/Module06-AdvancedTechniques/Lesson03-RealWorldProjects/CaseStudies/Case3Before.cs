using System;
using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Controls;

namespace RealWorldProjectsApp.CaseStudies
{
    /// <summary>
    /// BEFORE: Non-optimized TreeView with eager loading
    /// Issues: All nodes loaded upfront, no virtualization, slow expansion
    /// </summary>
    public class Case3Before : UserControl
    {
        private TreeView _treeView;
        private int _nodeCount = 0;

        public Case3Before()
        {
            InitializeControl();
        }

        private void InitializeControl()
        {
            _treeView = new TreeView();

            // BAD: No virtualization
            VirtualizingPanel.SetIsVirtualizing(_treeView, false);

            Content = new ScrollViewer
            {
                Content = _treeView,
                VerticalScrollBarVisibility = ScrollBarVisibility.Auto
            };
        }

        public void BuildTree(int depth, int childrenPerNode)
        {
            _treeView.Items.Clear();
            _nodeCount = 0;

            // BAD: Build entire tree upfront - very slow for large trees
            var rootNode = new TreeViewItem
            {
                Header = "Root Node"
            };

            BuildNodeRecursive(rootNode, depth - 1, childrenPerNode);
            _nodeCount++;

            _treeView.Items.Add(rootNode);
        }

        private void BuildNodeRecursive(TreeViewItem parent, int remainingDepth, int childrenPerNode)
        {
            if (remainingDepth <= 0) return;

            // BAD: Create all children immediately
            for (int i = 0; i < childrenPerNode; i++)
            {
                var childNode = new TreeViewItem
                {
                    Header = $"Node {_nodeCount++}"
                };

                // BAD: Recursively build all descendants
                BuildNodeRecursive(childNode, remainingDepth - 1, childrenPerNode);

                parent.Items.Add(childNode);
            }
        }

        public int GetNodeCount()
        {
            return _nodeCount;
        }

        public void ExpandAll()
        {
            // BAD: Expanding all nodes is very slow
            foreach (TreeViewItem item in _treeView.Items)
            {
                ExpandRecursive(item);
            }
        }

        private void ExpandRecursive(TreeViewItem item)
        {
            item.IsExpanded = true;
            foreach (var child in item.Items)
            {
                if (child is TreeViewItem childItem)
                {
                    ExpandRecursive(childItem);
                }
            }
        }

        public void CollapseAll()
        {
            foreach (TreeViewItem item in _treeView.Items)
            {
                CollapseRecursive(item);
            }
        }

        private void CollapseRecursive(TreeViewItem item)
        {
            item.IsExpanded = false;
            foreach (var child in item.Items)
            {
                if (child is TreeViewItem childItem)
                {
                    CollapseRecursive(childItem);
                }
            }
        }
    }
}
