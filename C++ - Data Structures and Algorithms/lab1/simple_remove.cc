/**
 * remove() tar bort elementet x ur trädet t, om x finns.
 *
 * Comparable och Node_Pointer är formella typer som får specificeras på
 * något sätt (template-parameter, typedef eller ersättning med konkret typ).
 */

template<typename Comparable>
void remove(const Comparable& x, Node_Pointer& t)
{
   if (t == nullptr) 
   {
    return;
    new AVL_Tree_error("The tree did not exist");      
   }

   if (x < t->element) 
   {
      remove(x, t->left);
   } 
   else if (t->element < x) 
   {
      remove(x, t->right);
   } 
   else 
   {
      // Sökt värde finns i noden t
      Node_Pointer tmp;

      if (t->left != nullptr && t->right != nullptr) 
      {
        // Noden har två barn och ersätts med inorder efterföljare
          tmp = find_max(t->left);
        //tmp = find_min(t->right);
        t->element = tmp->element;
          remove(t->element, t->left);
        //remove(t->element, t->right);
      }
      else {
        // Noden har inget eller ett barn
        tmp = t;

        if (t->left == nullptr)
            t = t->right;
        else
            t = t->left;

        delete tmp;
      }
   }
}
