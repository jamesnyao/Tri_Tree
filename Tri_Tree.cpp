#include "Tri_Tree.h"

using namespace std;

// Default construction (values are undefined)
template <class T>
Tri_Tree<T>::Tri_Tree()
{
	left = nullptr;
	middle = nullptr;
	right = nullptr;
	flags = 0;
}

// Value constructor with one value
template <class T>
Tri_Tree<T>::Tri_Tree(const T &value) : Tri_Tree()
{
	value_a = value;
	flags = 1;
}

// Value constructor with two values
template <class T>
Tri_Tree<T>::Tri_Tree(const T &_value_a, const T &_value_b) : Tri_Tree()
{
	value_a = _value_a;
	value_b = _value_b;
	flags = 3;
}

// Finds if a value exists in this tree
template <class T>
bool Tri_Tree<T>::find(const T &value) const
{
	// cases where both values are valid
	if (flags == 3) {
		if (value == value_a) return true;
		if (value == value_b) return true;
		if ((value < value_a) && left)
			return left->find(value);
		if (((value > value_a) && (value < value_b)) && middle)
			return middle->find(value);
		if ((value > value_b) && right)
			return right->find(value);

	// cases where only value_b is valid
	} else if (flags == 2) {
		if (value == value_b) return true;
		if ((value < value_b) && (left && middle))
			return left->find(value) || middle->find(value);
		if ((value < value_b) && middle)
			return middle->find(value);
		if ((value < value_b) && left)
			return left->find(value);
		if ((value > value_b) && right)
			return right->find(value);

	// cases where only value_a is valid
	} else if (flags == 1) {
		if (value == value_a) return true;
		if ((value > value_a) && (middle && right))
			return middle->find(value) || right->find(value);
		if ((value > value_a) && middle)
			return middle->find(value);
		if ((value > value_a) && right)
			return right->find(value);
		if ((value < value_a) && left)
			return left->find(value);

	// cases where no value is valid
	} else {
		if (left && middle && right)
			return (left->find(value) || middle->find(value) || right->find(value));
		if (left && middle)
			return (left->find(value) || middle->find(value));
		if (middle && right)
			return (middle->find(value) || right->find(value));
		if (left && right)
			return (left->find(value) || right->find(value));
		if (left) return left->find(value);
		if (middle) return middle->find(value);
		if (right) return right->find(value);
	}

	// false if no values were found to be equal to the argument
	return false;
}

// Inserts a new value into the tree recursively
// Checks value in comparison to value_a and value_b, and places appropriately
template <class T>
bool Tri_Tree<T>::insert(const T &value)
{
	// if value repeats, deny the insert
	if (find(value)) return false;
	
	// Case were either value_a or value_b is empty
	// value_a is empty
	if (!(flags & 1)) {
		if (right && (value > value_b)) return right->insert(value);
		if (!right && (value > value_b)) return (right = new Tri_Tree<T>(value));

		// Check if value falls between the values in left and middle subtrees
		// inserts into left subtree if value is too small (smaller than the values already in left subtree)
		if (	(	(left && middle) && !(
			(	(	(	(left->flags == 3) || (left->flags == 2)
					) && (value > left->value_b)
				) || ( (left->flags == 1) && (value > left->value_a) )
			) && (	(	(	(middle->flags == 3) || (middle->flags == 1)
						) && (value < middle->value_a)
					) || ( (middle->flags == 2) && (value < middle->value_b) )
			)	)	) || ( left && !(
				(	(	(left->flags == 3) || (left->flags == 2)
					) && (value > left->value_b)
				) || ( (left->flags == 1) && (value > left->value_a) )
			)	)
		) return left->insert(value);
		// inserts into middle subtree if value is too large (larger than the values already in middle subtree)
		if ( middle && !(
			(	(	(middle->flags == 3) || (middle->flags == 1)
				) && (value < middle->value_a)
			) || ( (middle->flags == 2) && (value < middle->value_b) )
			)
		) return middle->insert(value);

		// else set value_a to value
		value_a = value;
		flags |= 1;
		return true;
	}
	// value_b is empty
	if (!(flags & 2)) {
		if (left && (value < value_a)) return left->insert(value);
		if (!left && (value < value_a)) return (left = new Tri_Tree<T>(value));
		
		// Check if value falls between the values in right and middle subtrees
		// inserts into right subtree if value is too large (larger than the values already in right subtree)
		if (	( 	(right && middle) && !(
			(	(	(	(right->flags == 3) || (right->flags == 1)
					) && (value < right->value_a)
				) || ( (right->flags == 2) && (value < right->value_b) )
			) && (	(	(	(middle->flags == 3) || (middle->flags == 2)
						) && (value > middle->value_b)
					) || ( (middle->flags == 2) && (value > middle->value_a) )
			)	)	) || ( right && !(
				(	(	(right->flags == 3) || (right->flags == 1)
					) && (value < right->value_a)
				) || ( (right->flags == 2) && (value < right->value_b) )
			)	)
		) return right->insert(value);
		// inserts into middle subtree if value is too small (smaller than the values already in middle subtree)
		if ( middle && !(
			(	(	(middle->flags == 3) || (middle->flags == 2)
				) && (value > middle->value_b)
			) || ( (middle->flags == 2) && (value > middle->value_a) )
			)
		) return middle->insert(value);

		// else set value_b to value
		value_b = value;
		flags |= 2;
		return true;
	}

	// Case where both values are full
	// left
	if (value < value_a) {
		if (!left) return (left = new Tri_Tree<T>(value));
		return left->insert(value);
	}
	// right
	if (value > value_b) {
		if (!right) return (right = new Tri_Tree<T>(value));
		return right->insert(value);
	}
	// middle
	if (!middle) return (middle = new Tri_Tree<T>(value));
	return middle->insert(value);
}

// Combine the values of a tree into this tree
template <class T>
bool Tri_Tree<T>::insert(Tri_Tree<T>* tree)
{
	if (check_if_repeat(tree)) insert_tree_helper(tree);
	return false;
}

// Inserts every element in a tree into this tree
template <class T>
void Tri_Tree<T>::insert_tree_helper(Tri_Tree<T>* tree)
{
	// inserts every value in the argument tree into this tree by going to the leaves
	if (tree->left) insert_tree_helper(tree->left);
	if (tree->middle) insert_tree_helper(tree->middle);
	if (tree->right) insert_tree_helper(tree->right);

	// then inserting and unrolling the stack
	if (tree->flags & 1) insert(tree->value_a);
	if (tree->flags & 2) insert(tree->value_b);
}

// Returns true if no values in tree are equal to any values in this
template <class T>
bool Tri_Tree<T>::check_if_repeat(Tri_Tree<T>* tree) const
{
	// checks if values repeat in this node
	if ((tree->flags & 1) && find(tree->value_a)) return false;
	if ((tree->flags & 2) && find(tree->value_b)) return false;
	
	// else check subtrees
	if (tree->left) return check_if_repeat(tree->left);
	if (tree->middle) return check_if_repeat(tree->middle);
	if (tree->right) return check_if_repeat(tree->right);
	return true;
}

// Removes a value from the tree
template <class T>
bool Tri_Tree<T>::remove(const T& value)
{
	// check if value is in this node, remove it if so
	if ((flags & 1) && (value == value_a)) { flags &= 2; return true; }
	if ((flags & 2) && (value == value_b)) { flags &= 1; return true; }	
	
	// check if value is in left tree
	if (left && left->find(value)) {
		// value_a
		if ((left->flags & 1) && (value == left->value_a)) {
			// if both value_a and value_b are valid
			if (left->flags & 2) { left->flags = 2; return true; }
			// if only value_a is valid and there are no subtrees
			if (!left->left && !left->middle && !left->right) {
				delete left;
				left = nullptr;
				return true;
			}
			// otherwise remove value and repopulate the spot
			left->flags &= 2;
			return true;
		}
		// value_b
		if ((left->flags & 2) && (value == left->value_b)) {
			// if both value_a and value_b are valid
			if (left->flags & 1) { left->flags = 1; return true; }
			// if only value_a is valid and there are no subtrees
			if (!left->left && !left->middle && !left->right) {
				delete left;
				left = nullptr;
				return true;
			}
			// otherwise remove value and repopulate the spot
			left->flags &= 1;
			return true;
		}

		// if value is not in this node, check subtrees
		if (left->left && left->left->find(value))
			return left->left->remove(value);
		if (left->middle && left->middle->find(value))
			return left->middle->remove(value);
		if (left->right && left->right->find(value))
			return left->right->remove(value);

	// check if value is in middle tree
	} else if (middle && middle->find(value)) {
		// value_a
		if ((middle->flags & 1) && (value == middle->value_a)) {
			// if both value_a and value_b are valid
			if (middle->flags & 2) { middle->flags = 2; return true; }
			// if only value_a is valid and there are no subtrees
			if (!middle->left && !middle->middle && !middle->right) {
				delete middle;
				middle = nullptr;
				return true;
			}
			// otherwise remove value and repopulate the spot
			middle->flags &= 2;
			return true;
		}
		// value_b
		if ((middle->flags & 2) && (value == middle->value_b)) {
			// if both value_a and value_b are valid
			if (middle->flags & 1) { middle->flags = 1; return true; }
			// if only value_a is valid and there are no subtrees
			if (!middle->left && !middle->middle && !middle->right) {
				delete middle;
				middle = nullptr;
				return true;
			}
			// otherwise remove value and repopulate the spot
			middle->flags &= 1;
			return true;
		}

		// if value is not in this node, check subtrees
		if (middle->left && middle->left->find(value))
			return middle->left->remove(value);
		if (middle->middle && middle->middle->find(value))
			return middle->middle->remove(value);
		if (middle->right && middle->right->find(value))
			return middle->right->remove(value);

	// check if value is in right tree
	} else if (right && right->find(value)) {
		// value_a
		if ((right->flags & 1) && (value == right->value_a)) {
			// if both value_a and value_b are valid
			if (right->flags & 2) { right->flags = 2; return true; }
			// if only value_a is valid and there are no subtrees
			if (!right->left && !right->middle && !right->right) {
				delete right;
				right = nullptr;
				return true;
			}
			// otherwise remove value and repopulate the spot
			right->flags &= 2;
			return true;
		}
		// value_b
		if ((right->flags & 2) && (value == right->value_b)) {
			// if both value_a and value_b are valid
			if (right->flags & 1) { right->flags = 1; return true; }
			// if only value_a is valid and there are no subtrees
			if (!right->left && !right->middle && !right->right) {
				delete right;
				right = nullptr;
				return true;
			}
			// otherwise remove value and repopulate the spot
			right->flags &= 1;
			return true;
		}

		// if value is not in this node, check subtrees
		if (right->left && right->left->find(value))
			return right->left->remove(value);
		if (right->middle && right->middle->find(value))
			return right->middle->remove(value);
		if (right->right && right->right->find(value))
			return right->right->remove(value);
		
	// nothing is removed
	} else return false;
}

// Prints out this tree
template <class T>
void Tri_Tree<T>::display() const
{
	display_helper();
	cout << endl;
}

// Prints out tree in order from left to right
template <class T>
void Tri_Tree<T>::display_helper() const
{
	// order: left tree, value_a, middle tree, value_b, right tree
	if (left) left->display_helper();
	if (flags & 1) cout << value_a << ' ';
	if (middle) middle->display_helper();
	if (flags & 2) cout << value_b << ' ';
	if (right) right->display_helper();
}

// Returns the pointer to the left Tri_Tree
template <class T>
Tri_Tree<T>* Tri_Tree<T>::get_left() const
{
	return left;
}

// Returns the pointer to the middle Tri_Tree
template <class T>
Tri_Tree<T>* Tri_Tree<T>::get_middle() const
{
	return middle;
}

// Returns the pointer to the right Tri_Tree
template <class T>
Tri_Tree<T>* Tri_Tree<T>::get_right() const
{
	return right;
}

// Gets the first value in this tree (value_a)
template <class T>
T Tri_Tree<T>::get_value_a() const
{
	return value_a;
}

// Gets the second value in this tree (value_b)
template <class T>
T Tri_Tree<T>::get_value_b() const
{
	return value_b;
}

// Sets the left pointer to tree
template <class T>
void Tri_Tree<T>::set_left(Tri_Tree<T>* tree)
{
	if (left) delete left;
	left = tree;
}

// Sets the middle pointer to tree
template <class T>
void Tri_Tree<T>::set_middle(Tri_Tree<T>* tree)
{
	if (middle) delete middle;
	middle = tree;
}

// Sets the right pointer to tree
template <class T>
void Tri_Tree<T>::set_right(Tri_Tree<T>* tree)
{
	if (right) delete right;
	right = tree;
}

// Sets the values of the current tree to _value_a and _value_b
template <class T>
void Tri_Tree<T>::set_values(const T& _value_a, const T& _value_b)
{
	value_a = _value_a;
	value_b = _value_b;
	flags = 3;
}

// Sets value_a of the current tree to value
template <class T>
void Tri_Tree<T>::set_value_a(const T& value)
{
	value_a = value;
	flags |= 1;
}

// Sets value_b of the current tree to value
template <class T>
void Tri_Tree<T>::set_value_b(const T& value)
{
	value_b = value;
	flags |= 2;
}

// Deletes the values of the current tree
template <class T>
bool Tri_Tree<T>::del_values()
{
	if (!flags) return false;
	flags = 0;
	return true;
}

// Deletes value_a of the current tree
template <class T>
bool Tri_Tree<T>::del_value_a()
{
	if (!(flags & 1)) return false; 
	flags &= 2;
	return true;
}

// Deletes value_b of the current tree
template <class T>
bool Tri_Tree<T>::del_value_b()
{
	if (!(flags & 2)) return false;
	flags &= 1;
	return true;
}

// There are 0 known bugs left to fix
template <class T>
void Tri_Tree<T>::debug()
{
	cout << "FLAGS: " << (int)flags
		<< " VALUE_A: " << value_a
		<< " VALUE_B: " << value_b << endl;
}

// Default destructor
template <class T>
Tri_Tree<T>::~Tri_Tree<T>()
{
	if (left) delete left;
	if (middle) delete middle;
	if (right) delete right;
}

// Add possible types
template class Tri_Tree<int>;
