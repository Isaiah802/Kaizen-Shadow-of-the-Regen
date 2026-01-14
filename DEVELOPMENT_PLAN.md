# 1-Year Development Plan: Nine Sols-Inspired Game

**Project Start:** January 2026  
**Target Completion:** January 2027  
**Development Hours:** ~15-20 hours/week (780-1040 hours total)

---

## Phase 1: Foundation (Months 1-2) - January - February 2026

### Month 1: Core Systems & Player Movement

**Goals:**
- ✓ Set up development environment (COMPLETED)
- Implement basic player character with sprite rendering
- Create fundamental movement system (walk, run)
- Implement physics system (gravity, ground collision)
- Basic camera that follows player

**Deliverables:**
- Player can move left/right on a flat surface
- Basic collision detection working
- 60 FPS stable performance

**Learning Focus:**
- SFML sprite management
- Basic game physics
- Entity-Component-System (ECS) architecture basics

**Estimated Time:** 60-80 hours

---

### Month 2: Combat Foundation

**Goals:**
- Implement player attack system (light attack)
- Create basic enemy AI (patrol, chase, attack)
- Health system for player and enemies
- Hit detection and damage system
- Basic UI (health bar, simple HUD)

**Deliverables:**
- Player can perform melee attacks
- At least 1 enemy type that can fight back
- Visual feedback for hits (screen shake, particle effects)
- Player can die and respawn

**Learning Focus:**
- State machines for combat
- AI pathfinding basics
- UI rendering with SFML

**Estimated Time:** 60-80 hours

---

## Phase 2: Core Gameplay Loop (Months 3-4) - March - April 2026

### Month 3: Advanced Movement & Platforming

**Goals:**
- Implement jump and double-jump
- Add dash mechanic (signature Nine Sols feature)
- Wall slide and wall jump
- Platform collision (one-way platforms, moving platforms)
- Ledge grabbing

**Deliverables:**
- Fluid platforming controls
- 2-3 small test levels showcasing mechanics
- Tight, responsive controls (input buffering)

**Learning Focus:**
- Advanced collision detection (tilemap, slopes)
- Input buffering and coyote time
- Juice and game feel (animation curves, tweening)

**Estimated Time:** 70-90 hours

---

### Month 4: Expanded Combat System

**Goals:**
- Combo system (3-hit chain)
- Heavy attacks and launchers
- Dodge/parry mechanics
- 2-3 additional enemy types with varied behaviors
- Enemy spawn system

**Deliverables:**
- Satisfying combat with multiple attack options
- Enemies require different strategies
- Basic combo counter UI

**Learning Focus:**
- Animation state machines
- Frame data and hitbox/hurtbox systems
- Enemy behavior trees

**Estimated Time:** 70-90 hours

---

## Phase 3: Systems & Polish (Months 5-6) - May - June 2026

### Month 5: Resource Management & Progression

**Goals:**
- Asset management system (load sprites, sounds efficiently)
- Save/load system
- Basic skill tree or upgrade system
- Currency and shop system
- Inventory system (if applicable)

**Deliverables:**
- Player progress saves between sessions
- Upgrades affect gameplay meaningfully
- Resource files organized and easily extensible

**Learning Focus:**
- Serialization (JSON/Binary)
- Resource pooling and optimization
- Game balance fundamentals

**Estimated Time:** 60-80 hours

---

### Month 6: Audio & Visual Polish

**Goals:**
- Background music system with smooth transitions
- Sound effects for all actions
- Particle system overhaul (hit effects, dust, trails)
- Screen effects (damage flash, slow-mo on heavy hits)
- Improved animations (12+ frames for main actions)

**Deliverables:**
- Game feels impactful and responsive
- Audio feedback for every action
- Visual clarity in combat situations

**Learning Focus:**
- Audio programming (SFML Audio module)
- Particle system optimization
- Animation principles (squash & stretch, anticipation)

**Estimated Time:** 60-80 hours

---

## Phase 4: Content Creation (Months 7-9) - July - September 2026

### Month 7: Level Design Foundations

**Goals:**
- Create level editor or workflow
- Design 3-5 full game areas/levels
- Implement checkpoints and respawn system
- Background parallax layers
- Environmental hazards (spikes, pits, etc.)

**Deliverables:**
- Playable levels with increasing difficulty
- Visual distinction between areas
- Clear player guidance (environmental storytelling)

**Learning Focus:**
- Level design principles
- Player psychology and flow
- Art asset creation/sourcing

**Estimated Time:** 80-100 hours

---

### Month 8: Boss Design

**Goals:**
- Design and implement 2-3 boss fights
- Boss attack patterns and phases
- Boss UI (health bar, phase indicators)
- Cinematic transitions (boss intro/outro)
- Boss-specific arenas

**Deliverables:**
- Challenging but fair boss encounters
- Memorable attack patterns
- Victory feels earned

**Learning Focus:**
- Boss design patterns (telegraphing, pattern recognition)
- State machine complexity
- Difficulty balancing

**Estimated Time:** 80-100 hours

---

### Month 9: Additional Content & Systems

**Goals:**
- Implement 4-6 more enemy types
- Special abilities/skills system
- Environmental puzzles
- Collectibles and secrets
- Lore/story delivery system (if applicable)

**Deliverables:**
- Rich enemy variety
- Replayability through secrets
- Optional challenges for skilled players

**Learning Focus:**
- Content pipeline optimization
- Puzzle design principles
- Pacing and content density

**Estimated Time:** 70-90 hours

---

## Phase 5: Integration & Testing (Months 10-11) - October - November 2026

### Month 10: Full Game Integration

**Goals:**
- Connect all levels into cohesive experience
- Implement game flow (main menu, pause, game over)
- Difficulty settings
- Accessibility options (control remapping, visual assists)
- Performance optimization pass

**Deliverables:**
- Complete playthrough possible start-to-finish
- Stable performance on target hardware
- Professional UI/UX

**Learning Focus:**
- Game state management
- Performance profiling
- UX design principles

**Estimated Time:** 70-90 hours

---

### Month 11: Testing & Bug Fixing

**Goals:**
- Internal playtesting (complete game multiple times)
- External playtesting (5-10 testers)
- Bug tracking and fixing
- Balance adjustments based on feedback
- Softlock prevention

**Deliverables:**
- Bug-free core experience
- Balanced difficulty curve
- Smooth progression without frustration spikes

**Learning Focus:**
- QA methodologies
- Player feedback interpretation
- Iteration based on data

**Estimated Time:** 60-80 hours

---

## Phase 6: Polish & Launch Prep (Month 12) - December 2026

### Month 12: Final Polish & Release

**Goals:**
- Visual polish pass (consistent art style)
- Audio polish (mixing, levels, atmospheric sounds)
- Final optimization
- Achievements/Steam integration (if applicable)
- Trailer and marketing materials
- Documentation (credits, readme)

**Deliverables:**
- Polished, release-ready game
- Promotional materials
- Distribution build (itch.io, Steam, etc.)

**Learning Focus:**
- Marketing basics
- Build deployment
- Post-launch planning

**Estimated Time:** 60-80 hours

---

## Technical Milestones Checklist

### Core Engine Features
- [ ] Entity Component System (ECS)
- [ ] Physics engine (custom or Box2D integration)
- [ ] Animation system with state machines
- [ ] Particle system
- [ ] Camera system (follow, shake, zoom)
- [ ] Input system with rebinding
- [ ] Save/Load system
- [ ] Audio manager
- [ ] Resource manager
- [ ] Scene/Level management

### Gameplay Features
- [ ] Player movement (8-way with physics)
- [ ] Jump + double jump
- [ ] Dash mechanics
- [ ] Wall slide/jump
- [ ] Combat system (combos, special moves)
- [ ] Dodge/parry
- [ ] Enemy AI (5+ enemy types)
- [ ] Boss AI (2-3 bosses)
- [ ] Health/damage system
- [ ] Upgrade/skill system
- [ ] Collectibles
- [ ] Checkpoints

### Content Goals
- [ ] 10-15 playable levels
- [ ] 2-3 major boss fights
- [ ] 6-8 enemy types
- [ ] 20+ animations per character
- [ ] 50+ sound effects
- [ ] 5-8 music tracks
- [ ] 3-5 distinct environments

---

## Asset Creation Timeline

### Art Assets
- **Month 1-2:** Player sprite sheets (idle, walk, jump, attack)
- **Month 3-4:** Enemy sprites, basic tilesets
- **Month 5-6:** Environment art, parallax backgrounds
- **Month 7-8:** Boss sprites, special effects
- **Month 9-10:** Polish pass, additional animations
- **Month 11-12:** UI art, marketing materials

### Audio Assets
- **Month 4-5:** Core SFX (footsteps, attacks, hits)
- **Month 6-7:** Environment ambience, enemy sounds
- **Month 8:** Boss music and SFX
- **Month 9-10:** UI sounds, additional tracks
- **Month 11-12:** Final mix and polish

**Art Style Recommendation:**
- 2D hand-drawn sprites (64x64 to 128x128)
- Pixel art or high-res sprites (depends on team skill)
- Consistent color palette across game
- Consider hiring freelance artists or using placeholder assets initially

---

## Risk Mitigation

### High-Risk Areas
1. **Scope Creep:** Stick to core features, save "nice-to-haves" for updates
2. **Art Assets:** Start with programmer art, upgrade gradually
3. **Performance:** Profile regularly, optimize early
4. **Burnout:** Take breaks, maintain work-life balance

### Contingency Plans
- **If Behind Schedule:** Cut optional content (extra levels, collectibles)
- **If Technical Blockers:** Simplify systems, use existing libraries
- **If Art Pipeline Stalls:** Use asset stores, simple geometric shapes
- **If Alone:** Reduce scope to 6-8 months of core content

---

## Tools & Resources Needed

### Development Tools
- ✓ Visual Studio Code / Visual Studio
- ✓ CMake
- ✓ Git for version control
- [ ] Debugger proficiency (GDB)
- [ ] Asset tools (Aseprite/GIMP for sprites)
- [ ] Level editor (Tiled or custom)

### Learning Resources
- C++ game development books/tutorials
- SFML game development series
- Game design patterns (Robert Nystrom's "Game Programming Patterns")
- Game feel resources (Jan Willem Nijman's talks)
- Nine Sols gameplay analysis videos

### Community
- SFML forums/Discord
- r/gamedev and r/devblogs
- Local game dev meetups
- Beta testing community

---

## Success Metrics

### By End of Year 1
- **Technical:** Stable 60 FPS, <50 known bugs
- **Content:** 30-60 minutes of polished gameplay
- **Quality:** 7/10 or higher in playtester reviews
- **Learning:** Proficient in C++ game architecture
- **Portfolio:** Showable vertical slice for resume

### Stretch Goals
- Steam release
- 100+ wishlists
- Positive community feedback
- Foundation for sequel or expanded edition

---

## Monthly Review Questions

Ask yourself at end of each month:
1. Did I hit my deliverables?
2. What went better than expected?
3. What was harder than expected?
4. Do I need to adjust scope or timeline?
5. Am I still excited about the project?
6. What did I learn this month?

---

## Notes

- **Flexibility:** This is a guide, not a contract. Adjust as you learn.
- **Quality > Quantity:** Better to have 5 great levels than 20 mediocre ones.
- **Playtest Early:** Get feedback starting Month 3-4, not Month 11.
- **Document Everything:** Keep a dev blog or journal for learning.
- **Iterate:** First versions will be rough. Refinement takes time.

---

**Remember:** Most indie games take 2-4 years. Completing a polished vertical slice in 1 year is a huge achievement. Focus on learning and creating something you're proud of, even if it's not a full game.

Good luck! 🎮
