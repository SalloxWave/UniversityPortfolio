template<typename Comparable>
void AVL_Tree_Node<Comparable>::
remove(const Comparable& x, Node_Pointer& t) {

  if (t == nullptr)
  {
    return;
  }

  //Go left
  if (x < t->element)
  {
    remove(x, t->left);
  }
  //Go right
  else if(x > t->element)
  {
    remove(x, t->right);
  }
  //Node with value x found
  else
  {
    // Sökt värde finns i noden t
    Node_Pointer tmp;

    //The node has 2 children
    if (t->left != nullptr && t->right != nullptr)
    {
      // Noden har två barn och ersätts med inorder efterföljare
      //Node gets replaced with inorder presuccesor
      tmp = find_min(t->right);
      t->element = tmp->element;
      remove(t->element, t->right);
    }
    //None or one child
    else
    {
      tmp = t;

      if (t->left == nullptr)
        t = t->right;
      else
        t = t->left;

      delete tmp;

      //To avoid checking for balance on potential nullpointer
      return;
    }
  }
  //Needs balance
  if (abs(node_height(t->left) - node_height(t->right)) == 2)
  {
    Node::balance_deletion(t);
  }
  else
  {
    calculate_height(t);
  }
}

template<typename Comparable>
void AVL_Tree_Node<Comparable>::balance_deletion(Node_Pointer& unbalanced)
{
  cout << unbalanced->element << " needs balancing" << endl;
  Node_Pointer tallestChild;
  Node_Pointer tallestGrandChild;
  if (node_height(unbalanced->left) > node_height(unbalanced->right))
  {
    tallestChild = unbalanced->left;
    if (node_height(tallestChild->left) >= node_height(tallestChild->right))
    {
      tallestGrandChild = tallestChild->left;
    }
    else
    {
      tallestGrandChild = tallestChild->right;
    }
  }
  else
  {
    tallestChild = unbalanced->right;
    if (node_height(tallestChild->right) >= node_height(tallestChild->left))
    {
      tallestGrandChild = tallestChild->right;
    }
    else
    {
      tallestGrandChild = tallestChild->left;
    }
  }

  //Child is on left side
  if (tallestChild->element < unbalanced->element)
  {
    //Left left -> Single rotate right
    if (tallestGrandChild->element < tallestChild->element)
    {
      single_rotate_with_left_child(unbalanced);
    }
    //Left Right -> Left rotation, Right rotation
    else
    {
      double_rotate_with_left_child(unbalanced);
    }
  }
  //Child is on right side
  else
  {
    //Right Right -> Single rotate left
    if (tallestGrandChild->element > tallestChild->element)
    {
      single_rotate_with_right_child(unbalanced);
    }
    //Right Left -> Right rotation, Left rotation
    else
    {
      double_rotate_with_right_child(unbalanced);
    }
  }
}