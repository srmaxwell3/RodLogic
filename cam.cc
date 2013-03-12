// 3D matrix of Cell's.

// A Cell's state can be undefined (e.g. "off the edge"); otherwise,
// it's state is one of a fixed, finite, number of states, probably
// represented by an integer value.

// A Cell's state represents a number of properties, including:

// a) limitations of when it's state can change.  In a 'normal' CA,
// the state of every Cell is updated with every clock tick, or
// generation.  In CAM, however, a state can limit updates to
// particular clock ticks, especially particular multiples of ticks.
// E.g. why bother updating Cells making up a Phase A latching rod at
// any time other than when the current clock 'time' says we're
// latching or unlatching Stage A logic rods?

// b) stage of operation.


class CellProperties {
};

class Cell {
  Cell(size_t initialState = 0): state(initialState) {}
};
