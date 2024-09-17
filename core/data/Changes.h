#ifndef CHANGES_H
#define CHANGES_H

template<typename A>
struct Changes {
  Changes(const A& previous, const A& next) : _previous(previous), _next(next) {}

  A previous() const { return _previous; }
  A next() const { return _next; }
private:
  A _previous, _next;
};

#endif // CHANGES_H