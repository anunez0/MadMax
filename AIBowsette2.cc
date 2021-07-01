#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Bowsette2


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
  vector<vector<int> > gr;
  vector<vector<vector<int> > > gc;
  vector<vector<int> > gw;
  vector<vector<int> > gs;
  vector<Pos> vc;
  map<int, Pos> mp;
  
  
  void bfs(vector<vector<int> >& g, const Pos& p) {
    g[p.i][p.j] = 0;
    vector<vector<bool> > visited(rows(), vector<bool>(cols(), false));
    visited[p.i][p.j] = true;
    queue<Pos> q;
    q.push(p);
    while (not q.empty()) {
      Pos r = q.front();
      q.pop();
      for (int i = 0; i < 8; ++i) {
        Pos s = r + Dir(i);
        if (pos_ok(s) and (cell(s).type == Desert or cell(s).type == Road or cell(s).type == City) and not visited[s.i][s.j]) {
          if (g[s.i][s.j] > g[r.i][r.j] + 1) g[s.i][s.j] = g[r.i][r.j] + 1;
          q.push(s);
          visited[s.i][s.j] = true;
        }
      }
    }
  }
  
  
  void bfs_city(vector<vector<int> >& g, const Pos& p, vector<vector<bool> >& visited) {
    bfs(g, p);
    visited[p.i][p.j] = true;
    queue<Pos> q;
    q.push(p);
    while (not q.empty()) {
      Pos r = q.front();
      q.pop();
      for (int i = 0; i < 8; ++i) {
        Pos s = r + Dir(i);
        if (pos_ok(s) and cell(s).type == City and not visited[s.i][s.j]) {
          bfs(g, s);
          q.push(s);
          visited[s.i][s.j] = true;
        }
      }
    }
  }
  
  
  void bfs_road(vector<vector<int> >& g, const Pos& p) {
    g[p.i][p.j] = 0;
    vector<vector<bool> > visited(rows(), vector<bool>(cols(), false));
    visited[p.i][p.j] = true;
    queue<Pos> q;
    q.push(p);
    while (not q.empty()) {
      Pos r = q.front();
      q.pop();
      for (int i = 0; i < 8; ++i) {
        Pos s = r + Dir(i);
        if (pos_ok(s) and (cell(s).type == Desert or cell(s).type == Road) and not visited[s.i][s.j]) {
          if (g[s.i][s.j] > g[r.i][r.j] + 1) g[s.i][s.j] = g[r.i][r.j] + 1;
          q.push(s);
          visited[s.i][s.j] = true;
        }
      }
    }
  }
  
  
  void bfs_station(vector<vector<int> >& g, const Pos& p) {
    g[p.i][p.j] = 0;
    vector<vector<bool> > visited(rows(), vector<bool>(cols(), false));
    visited[p.i][p.j] = true;
    queue<Pos> q;
    q.push(p);
    while (not q.empty()) {
      Pos r = q.front();
      q.pop();
      for (int i = 0; i < 8; ++i) {
        Pos s = r + Dir(i);
        if (pos_ok(s) and cell(s).type == Road and not visited[s.i][s.j]) {
          if (g[s.i][s.j] > g[r.i][r.j] + 1) g[s.i][s.j] = g[r.i][r.j] + 1;
          q.push(s);
          visited[s.i][s.j] = true;
        }
      }
    }
  }
  
  
  Dir search(const vector<vector<int> >& g, const Pos& p, Dir& d) {
    int min = g[p.i][p.j];
    for (int i = 0; i < 8; ++i) {
      Pos q = p + Dir(i);
      bool b = false;
      for (map<int, Pos>::const_iterator it = mp.begin(); it != mp.end() and not b; ++it) {
        if (it->second == q) b = true;
      }
      if (pos_ok(q) and g[q.i][q.j] < min and not b and (cell(q).id == -1 or unit(cell(q).id).player != me())) {
        d = Dir(i);
        min = g[q.i][q.j];
      }
    }
    return d;
  }
  
  
  bool next_to_car(const Pos& p, Dir& d) {
    bool b = false;
    for (int i = 0; i < 8 and not b; ++i) {
      Pos q = p + Dir(i);
      if (pos_ok(q) and cell(q).id != -1 and unit(cell(q).id).type == Car and unit(cell(q).id).player != me()) {
        b = true;
        d = Dir((i + 4)%8);
      }
    }
    if (!pos_ok(p + d)) d = None;
    else {
      bool b2 = false;
      for (map<int, Pos>::const_iterator it = mp.begin(); it != mp.end() and not b2; ++it) {
        if (it->second == p + d) b2 = true;
      }
      if (b2 or (cell(p + d).id != -1 and unit(cell(p + d).id).player == me())) d = None;
    }
    return b;
  }
  
  
  bool next_to_warrior(const Pos& p, Dir& d) {
    bool b = false;
    for (int i = 0; i < 8 and not b; ++i) {
      Pos q = p + Dir(i);
      bool b2 = false;
      for (map<int, Pos>::const_iterator it = mp.begin(); it != mp.end() and not b2; ++it) {
        if (it->second == q) b2 = true;
      }
      if (pos_ok(q) and cell(q).id != -1 and unit(cell(q).id).type == Warrior and unit(cell(q).id).player != me() and (cell(q).type != City or unit(cell(q).id).water < unit(cell(p).id).water) and not b2) {
        b = true;
        d = Dir(i);
      }
    }
    return b;
  }
  
  
  int allies_in_city(const Pos& p) {
    int n = 1;
    vector<vector<bool> > visited(rows(), vector<bool>(cols(), false));
    visited[p.i][p.j] = true;
    queue<Pos> q;
    q.push(p);
    vector<int> vwar = warriors(me());
    int nwar = vwar.size();
    while (not q.empty()) {
      Pos r = q.front();
      q.pop();
      for (int i = 0; i < 8; ++i) {
        Pos s = r + Dir(i);
        if (pos_ok(s) and cell(s).type == City and not visited[s.i][s.j]) {
          for (map<int, Pos>::const_iterator it = mp.begin(); it != mp.end(); ++it) {
            if (it->second == s) ++n;
          }
          q.push(s);
          visited[s.i][s.j] = true;
        }
      }
    }
    return n;
  }
  
  
  Dir search_city(const Pos& p, Dir& d) {
    int i_min = -1;
    int min = rows();
    for (int i = 0; i < 8; ++i) {
      if (cell(vc[i]).owner != me() and gc[i][p.i][p.j] < min) {
        i_min = i;
        min = gc[i][p.i][p.j];
      }
    }
    if (i_min == -1) return None;
    return search(gc[i_min], p, d);
  }
  
  
  bool search_car_warrior(const Pos& p, Dir& d) {
    vector<vector<bool> > visited(rows(), vector<bool>(cols(), false));
    visited[p.i][p.j] = true;
    queue<Pos> q;
    q.push(p);
    bool b = false;
    while (not q.empty() and not b) {
      Pos r = q.front();
      q.pop();
      for (int i = 0; i < 8 and not b; ++i) {
        Pos s = r + Dir(i);
        if (pos_ok(s) and (cell(s).type == Desert or cell(s).type == Road) and not visited[s.i][s.j] and s.i <= p.i + 3 and s.i >= p.i - 3 and s.j <= p.j + 3 and s.j >= p.j - 3) {
          if (cell(s).id != -1 and unit(cell(s).id).type == Car and unit(cell(s).id).player != me()) {
            d = Dir((i + 4)%8);
            b = true;
          }
          q.push(s);
          visited[s.i][s.j] = true;
        }
      }
    }
    if (!pos_ok(p + d) or (cell(p + d).type != Desert and cell(p + d).type != Road and cell(p + d).type != City)) {
      if (pos_ok(p + Dir((int(d) + 1)%8)) and (cell(p + Dir((int(d) + 1)%8)).type == Desert or cell(p + Dir((int(d) + 1)%8)).type == Road or cell(p + Dir((int(d) + 1)%8)).type == City)) d = Dir((int(d) + 1)%8);
      else if (pos_ok(p + Dir((int(d) - 1)%8)) and (cell(p + Dir((int(d) - 1)%8)).type == Desert or cell(p + Dir((int(d) - 1)%8)).type == Road or cell(p + Dir((int(d) - 1)%8)).type == City)) d = Dir((int(d) - 1)%8);
    }
    if (!pos_ok(p + d)) d = None;
    else {
      bool b2 = false;
      for (map<int, Pos>::const_iterator it = mp.begin(); it != mp.end() and not b2; ++it) {
        if (it->second == p + d) b2 = true;
      }
      if (b2 or (cell(p + d).id != -1 and (unit(cell(p + d).id).player == me() or unit(cell(p + d).id).type == Car))) d = None;
    }
    return b;
  }
  
  
  bool search_car_car(const Pos& p, Dir& d) {
    vector<vector<bool> > visited(rows(), vector<bool>(cols(), false));
    visited[p.i][p.j] = true;
    queue<Pos> q;
    q.push(p);
    bool b = false;
    while (not q.empty() and not b) {
      Pos r = q.front();
      q.pop();
      for (int i = 0; i < 8 and not b; ++i) {
        Pos s = r + Dir(i);
        if (pos_ok(s) and (cell(s).type == Desert or cell(s).type == Road) and not visited[s.i][s.j] and s.i <= p.i + 2 and s.i >= p.i - 2 and s.j <= p.j + 2 and s.j >= p.j - 2) {
          if (cell(s).id != -1 and unit(cell(s).id).type == Car and unit(cell(s).id).player != me()) {
            d = Dir((i + 4)%8);
            b = true;
          }
          q.push(s);
          visited[s.i][s.j] = true;
        }
      }
    }
    if (!pos_ok(p + d) or (cell(p + d).type != Desert and cell(p + d).type != Road)) {
      if (pos_ok(p + Dir((int(d) + 1)%8)) and (cell(p + Dir((int(d) + 1)%8)).type == Desert or cell(p + Dir((int(d) + 1)%8)).type == Road)) d = Dir((int(d) + 1)%8);
      else if (pos_ok(p + Dir((int(d) - 1)%8)) and (cell(p + Dir((int(d) - 1)%8)).type == Desert or cell(p + Dir((int(d) - 1)%8)).type == Road)) d = Dir((int(d) - 1)%8);
    }
    if (!pos_ok(p + d)) d = None;
    else {
      bool b2 = false;
      for (map<int, Pos>::const_iterator it = mp.begin(); it != mp.end() and not b2; ++it) {
        if (it->second == p + d) b2 = true;
      }
      if (b2 or (cell(p + d).id != -1 and (unit(cell(p + d).id).player == me() or unit(cell(p + d).id).type == Car))) d = None;
    }
    return b;
  }
  
  
  Dir search_warrior(const Pos& p, Dir& d) {
    Pos pwar;
    vector<vector<bool> > visited(rows(), vector<bool>(cols(), false));
    visited[p.i][p.j] = true;
    queue<Pos> q;
    q.push(p);
    bool b = false;
    while (not q.empty() and not b) {
      Pos r = q.front();
      q.pop();
      for (int i = 0; i < 8 and not b; ++i) {
        Pos s = r + Dir(i);
        if (pos_ok(s) and (cell(s).type == Desert or cell(s).type == Road) and not visited[s.i][s.j]) {
          if (cell(s).id != -1 and unit(cell(s).id).type == Warrior and unit(cell(s).id).player != me()) {
            pwar = s;
            b = true;
          }
          if (cell(s).type == Road) q.push(s);
          visited[s.i][s.j] = true;
        }
      }
    }
    if (b) {
      vector<vector<int> > gwar(rows(), vector<int>(cols(), rows()));
      bfs_station(gwar, pwar);
      d = search(gwar, p, d);
    }
    return d;
  }
  
  
  bool lost(const Pos& p) {
    if (cell(p).type == Road) return false;
    for (int i = 0; i < 8; ++i) {
      Pos q = p + Dir(i);
      if (pos_ok(q) and cell(q).type == Road) return false;
    }
    return true;
  }
  
  
  void move_warriors() {
    vector<int> vwar = warriors(me());
    int n = vwar.size();
    for (int i = 0; i < n; ++i) {
      int id = vwar[i];
      if (can_move(id)) {
        Dir d = None;
        if (search_car_warrior(unit(id).pos, d)) command(id, d);
        else if (unit(id).water < gw[unit(id).pos.i][unit(id).pos.j] + 2*damage()) command(id, search(gw, unit(id).pos, d));
        else if (cell(unit(id).pos).type == City and (cell(unit(id).pos).owner != me() or allies_in_city(unit(id).pos) <= 3)) command(id, d);
        else if (next_to_warrior(unit(id).pos, d)) command(id, d);
        else command(id, search_city(unit(id).pos, d));
        mp[id] = unit(id).pos + d;
      }
    }
  }
  
  
  void move_cars() {
    vector<int> vcar = cars(me());
    int n = vcar.size();
    for (int i = 0; i < n; ++i) {
      int id = vcar[i];
      if (can_move(id)) {
        Dir d = None;
        if (search_car_car(unit(id).pos, d)) command(id, d);
        else if (lost(unit(id).pos)) command(id, search(gr, unit(id).pos, d));
        else if (unit(id).food < gs[unit(id).pos.i][unit(id).pos.j] + 4) command(id, search(gs, unit(id).pos, d));
        else if (next_to_warrior(unit(id).pos, d)) command(id, d);
        else command(id, search_warrior(unit(id).pos, d));
        mp[id] = unit(id).pos + d;
      }
    }
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    if (round() == 0) {
      gr = vector<vector<int> >(rows(), vector<int>(cols(), rows()));
      gc = vector<vector<vector<int> > >(8, vector<vector<int> >(rows(), vector<int>(cols(), rows())));
      gw = vector<vector<int> >(rows(), vector<int>(cols(), rows()));
      gs = vector<vector<int> >(rows(), vector<int>(cols(), rows()));
      vc = vector<Pos>(8);
      vector<vector<bool> > visited(rows(), vector<bool>(cols(), false));
      int k = 0;
      for (int i = 0; i < rows(); ++i) {
        for (int j = 0; j < cols(); ++j) {
          Pos p(i, j);
          if (cell(p).type == Road) bfs_road(gr, p);
          else if (cell(p).type == City and not visited[i][j]) {
            bfs_city(gc[k], p, visited);
            vc[k] = p;
            ++k;
          } else if (cell(p).type == Water) bfs(gw, p);
          else if (cell(p).type == Station) bfs_station(gs, p);
        }
      }
    }
    move_warriors();
    move_cars();
    mp.clear();
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
