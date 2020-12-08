#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <functional>

using namespace std;
using namespace __gnu_pbds;

using orderset = tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update>;
