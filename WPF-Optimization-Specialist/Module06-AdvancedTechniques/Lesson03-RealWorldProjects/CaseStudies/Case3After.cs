using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Controls;

namespace RealWorldProjectsApp.CaseStudies
{
    /// <summary>
    /// AFTER: Optimized TreeView with lazy loading and virtualization
    /// Improvements: Lazy loading, virtualization, dummy node pattern, fast expansion
    /// </summary>
    public class Case3After : UserControl
    {
        private TreeView _treeView;
        private int _maxDepth;
        private int _childrenPerNode;
        private int _nodeCount = 0;

        public Case3After()
        {
            InitializeControl();
        }

        private void InitializeControl()
        {
            _treeView = new TreeView();

            // GOOD: Enable virtualization
            VirtualizingPanel.SetIsVirtualizing(_treeView, true);
            VirtualizingPanel.SetVirtualizationMode(_treeView, VirtualizationMode.Recycling);

            Content = new ScrollViewer
            {
                Content = _treeView,
                VerticalScrollBarVisibility = ScrollBarVisibility.Auto,
                CanContentScroll = true
            };
        }

        public void BuildTree(int depth, int childrenPerNode)
        {
            _treeView.Items.Clear();
            _nodeCount = 0;
            _maxDepth = depth;
            _childrenPerNode = childrenPerNode;

            // GOOD: Only create root node
            var rootNode = CreateLazyNode("Root Node", 0);
            _nodeCount++;

            _treeView.Items.Add(rootNode);
        }

        private TreeViewItem CreateLazyNode(string header, int currentDepth)
        {
            var node = new TreeViewItem
            {
                Header = header,
                Tag = currentDepth // Store depth in Tag
            };

            // GOOD: Add dummy child if node can have children
            if (currentDepth < _maxDepth - 1)
            {
                node.Items.Add(null); // Dummy child shows expand arrow
            }

            // GOOD: Lazy load children when expanded
            node.Expanded += LazyNode_Expanded;

            return node;
        }

        private void LazyNode_Expanded(object sender, RoutedEventArgs e)
        {
            var node = sender as TreeViewItem;
            if (node == null) return;

            // GOOD: Check if already loaded (dummy child exists)
            if (node.Items.Count == 1 && node.Items[0] == null)
            {
                node.Items.Clear();

                int currentDepth = (int)(node.Tag ?? 0);

                // GOOD: Load children only when needed
                for (int i = 0; i < _childrenPerNode; i++)
                {
                    var childNode = CreateLazyNode($"Node {_nodeCount++}", currentDepth + 1);
                    node.Items.Add(childNode);
                }
            }

            // Prevent event from bubbling
            e.Handled = true;
        }

        public int GetNodeCount()
        {
            // Count only loaded nodes
            return _nodeCount;
        }

        public void ExpandAll()
        {
            // GOOD: Expand nodes efficiently
            foreach (TreeViewItem item in _treeView.Items)
            {
                ExpandRecursive(item);
            }
        }

        private void ExpandRecursive(TreeViewItem item)
        {
            // Trigger lazy loading by expanding
            item.IsExpanded = true;

            // Give UI time to update (lazy loading happens in Expanded event)
            Application.Current?.Dispatcher.Invoke(() => { }, System.Windows.Threading.DispatcherPriority.Background);

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
