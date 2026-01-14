# Game Design Document

**Project:** Nine Sols-Inspired Action Platformer  
**Started:** January 13, 2026  
**Engine:** C++ with SFML  

---

## Story & Setting

### Main Plot: Kaizen: Shadow of the Regent

**Protagonist:** Kaizen - A disciplined warrior trained in the ways of the blade, focused on melee combat with mastery of the parry. Unlocks supportive magic abilities to enhance combat without relying on ranged attacks.

**Mentor:** Masahiro - Wise and disciplined teacher who believes in peace and restraint. His execution by Lord Kurogane becomes the catalyst for Kaizen's journey.

**Antagonist:** Lord Kurogane - Tyrannical ruler who uses magical enhancements to maintain his iron grip on the land. Responsible for Masahiro's death and the suffering of countless innocents.

**Optional Allies:** Ayame and Raizo - Secondary playable characters unlocked later, each with unique combat styles and abilities.

**Setting:** A fantasy realm under tyrannical rule, featuring mountain dojos, destroyed villages, corrupted temples, and Kurogane's fortress capital.

**Conflict:** Kurogane's tyranny spreads across the land. When he orders a public execution and Masahiro tries to intervene peacefully, Kurogane executes him. Kaizen witnesses his mentor's death and vows revenge.

**Goal:** Avenge Masahiro's death by confronting Lord Kurogane while deciding whether to follow the path of vengeance or honor Masahiro's teachings of restraint and mercy.

**Key Story Beats:**

1. **Prologue - The Fall of the Dojo (~1 hour)**
   - Kaizen trains under Masahiro at a mountain dojo
   - Kurogane orders a public execution in the village
   - Masahiro attempts peaceful intervention
   - Kurogane executes Masahiro; Kaizen witnesses
   - Kaizen vows revenge
   - First mini-boss: Foreman/Executioner General

2. **Act 2 - The Tyrant's Reach (~4 hours)**
   - Kaizen travels through destroyed towns and temples
   - Witnesses the full extent of Kurogane's tyranny
   - Learns more about Masahiro's philosophy through townsfolk
   - Player choices affect side quests and story outcomes
   - Mini-bosses include Elite Guards and a former ally of Masahiro turned corrupt
   - Discovers hints about Kurogane's magical enhancements

3. **Act 3 - The Tyrant's Domain (~3 hours)**
   - Infiltrates Kurogane's fortress in the capital city
   - Navigates heavily guarded areas
   - Uncovers secrets about Kurogane's rise to power
   - Final story-tied mini-boss: Kurogane's personal lieutenant
   - Choices continue to shape the narrative

4. **Final Boss - Lord Kurogane (~1 hour)**
   - Three-phase battle in throne room
   - Kurogane taunts: "Your mentor was weak. He should have obeyed."
   - Intense parry-focused combat with magical enhancements
   - Player must master all learned abilities

5. **Epilogue - Resolution (~1 hour)**
   - Multiple endings based on player choices:
     - **Execution Ending:** Kill Kurogane, restore order through vengeance
     - **Mercy Ending:** Spare and exile him, honor Masahiro's teachings
     - **Legacy Ending:** Dismantle his rule completely, rebuild the land
   - Kaizen reflects on Masahiro's teachings
   - Cinematics show impact of player's choices throughout journey

### World Lore

**The Age of Tyranny:** Lord Kurogane rose to power through a combination of military might and forbidden magical enhancements. His reign brought prosperity to some but suffering to many, especially those who resisted his absolute authority.

**Masahiro's Philosophy:** The fallen mentor believed in discipline, restraint, and the power of choice. His teachings emphasized that true strength comes not from domination but from protecting the weak and showing mercy when possible.

**Magic System:** Supportive magic exists to enhance melee combat. These abilities are unlocked through story progression and represent Kaizen's growing mastery. Magic supplements swordplay but never replaces it—this is a pure melee experience.

**Hidden Knowledge:** Throughout the world, lore rooms and townsfolk hold secrets about:
- Kurogane's past and his path to power
- The source of his magical enhancements
- Masahiro's secret teachings and hidden techniques
- The history of the land before tyranny

**Player Agency:** Choices matter. Spare or kill enemies. Help townsfolk or focus solely on revenge. These decisions affect available quests, unlockable abilities, mini-boss behavior, and ultimately, the ending.

---

## Player Abilities & Mechanics

### Basic Movement
- **Walk/Run:** Left/Right movement with acceleration and deceleration
- **Jump:** Standard jump with responsive controls
- **Double Jump:** Air jump for extended mobility
- **Wall Slide:** [PLANNED] Slide down walls slowly
- **Wall Jump:** [PLANNED] Jump off walls to reach higher areas

### Combat Abilities

**Core Philosophy:** Kaizen is a **melee-only** warrior. All abilities enhance sword combat—no ranged attacks.

#### Light Attack
- **Input:** [Mouse Left Click / J key]
- **Damage:** Base melee damage
- **Speed:** Fast
- **Range:** Short (sword reach)
- **Combo:** Can chain into 3-hit combo
- **Description:** Quick horizontal slash, fundamental attack
- **Unlock:** Available from start

#### Heavy Attack
- **Input:** [Mouse Right Click / K key]
- **Damage:** 2x light attack damage
- **Speed:** Slow (requires wind-up)
- **Range:** Short (sword reach)
- **Special Properties:** Can break enemy guard, small knockback
- **Description:** Overhead strike with follow-through
- **Unlock:** Available from start

#### Special Abilities (Magic-Enhanced Melee)

##### Spirit Strike
- **Input:** [Q key]
- **Type:** Melee Enhancement
- **Cooldown:** 3 seconds
- **Description:** Infuse blade with spiritual energy for enhanced damage on next attack. Visual: blade glows with ethereal light
- **Unlock:** Prologue tutorial
- **Effect:** +50% damage on next melee strike

##### Echo Step
- **Input:** [E key while moving]
- **Type:** Mobility + Combat
- **Cooldown:** 5 seconds
- **Description:** Quick dash that leaves afterimage. Can dash through enemies for melee combo setup
- **Unlock:** Act 2 progression
- **Effect:** Brief invulnerability during dash, positions for combo

##### Blade Ward
- **Input:** [Hold R key]
- **Type:** Defensive Magic
- **Cooldown:** 8 seconds
- **Description:** Magical barrier appears around Kaizen, absorbing one hit
- **Unlock:** Act 2 progression
- **Effect:** Absorb next attack, reflects 25% damage back

##### Phantom Slash
- **Input:** [F key]
- **Type:** Multi-Enemy Melee
- **Cooldown:** 10 seconds
- **Description:** Spectral blade extension strikes multiple enemies in front of Kaizen
- **Unlock:** Act 3 progression
- **Effect:** Hits up to 3 enemies, reduced damage per target

##### Soul Resonance
- **Input:** [Automatic on perfect parry]
- **Type:** Parry Enhancement
- **Cooldown:** None (parry-triggered)
- **Description:** Successful perfect parry triggers soul energy, buffing next attack
- **Unlock:** Act 3 progression
- **Effect:** +100% damage on next attack after perfect parry

##### Optional Weapons / Abilities
- **Purchasable at shops** throughout the game
- **Magic Scrolls:** Additional enhancement variations
- **Special Weapons:** Alternative sword types with unique properties 

### Defensive Abilities

#### Parry (Core Mechanic)
- **Input:** [Spacebar - timed press before enemy attack lands]
- **Type:** Perfect parry system - **central to Kaizen's combat style**
- **Invincibility Frames:** 8 frames on successful parry
- **Description:** Deflect enemy attack with precise timing. Mastery of parry is essential.
- **Perfect Timing Window:** 6-frame window for perfect parry (strict but learnable)
- **Reward for Perfect:** 
  - Triggers Soul Resonance (when unlocked)
  - Brief slow-motion effect
  - Opens enemy to counterattack
  - +100% damage on next strike
- **Visual Feedback:** Screen flash, metallic clash sound, enemy staggers

#### Standard Block
- **Input:** [Hold Shift]
- **Damage Reduction:** 50% damage blocked
- **Stamina Cost:** Drains stamina while held, breaks if stamina depleted
- **Can Block:** Most standard attacks; boss heavy attacks break through
- **Description:** Defensive stance, less rewarding than parry but safer for learning 

### Ultimate Ability
- **Name:** [Your ultimate/signature move]
- **Input:** [Special button combination]
- **Charge Method:** [How does it charge? Combat? Time? Collectibles?]
- **Effect:** [What does it do?]
- **Visual:** [Describe the dramatic effect]
- **Description:** 

---

## Character Stats & Progression

### Base Stats
- **Health:** [Starting HP]
- **Attack Power:** [Base damage multiplier]
- **Defense:** [Damage reduction]
- **Speed:** [Movement speed]
- **Jump Height:** [How high can they jump?]

### Progression System
**Type:** [Skill tree? Linear upgrades? Currency-based? Experience levels?]

**Upgrade Categories:**
1. **Combat:** [Damage, combo extensions, new moves]
2. **Defense:** [HP, damage reduction, dodge improvements]
3. **Mobility:** [Movement speed, jump height, air control]
4. **Utility:** [Special abilities, cooldown reduction]

**Currency/Resources:**
- [Resource 1 name]: [How to obtain, what it's used for]
- [Resource 2 name]: [How to obtain, what it's used for]

---

## Enemy Types & Behaviors

### Basic Enemies

#### Kurogane's Guards
- **Health:** Low-Medium
- **Damage:** 1-2 hits to player
- **Behavior:** Patrol routes, aggressive when alerted
- **Attack Pattern:** Basic sword swings, telegraphed attacks
- **Weakness:** Predictable timing, vulnerable to parry
- **Drops:** Gold coins, health potions

#### Elite Guards
- **Health:** Medium
- **Damage:** 2-3 hits to player
- **Behavior:** Aggressive, pursue player
- **Attack Pattern:** Combo attacks, faster strikes
- **Weakness:** Leaves opening after combo finisher
- **Drops:** Gold coins, magic scrolls, rare materials

#### Magically-Enhanced Soldiers
- **Health:** Medium-High
- **Damage:** 3-4 hits to player
- **Behavior:** Uses magic-infused attacks
- **Attack Pattern:** Enhanced strikes with magical effects
- **Weakness:** Magic enhancement has cooldown, exploit gaps
- **Drops:** Rare materials, magic enhancement items

### Mini-Boss Encounters (~6-8 total)

#### Foreman / Executioner General (Prologue)
- **Location:** Village outskirts
- **Health:** Mini-boss tier
- **Story Significance:** Directly involved in execution that Kaizen witnesses
- **Attack Pattern:** Heavy overhead strikes, ground pounds
- **Teaches:** Basic parry timing, punishment windows

#### Former Ally of Masahiro (Act 2 - Story-Connected)
- **Location:** Corrupted temple
- **Health:** Medium mini-boss
- **Story Significance:** Once trained with Masahiro, now serves Kurogane
- **Attack Pattern:** Uses techniques similar to Kaizen's, emotional duel
- **Teaches:** Advanced parry timing, tests player's resolve
- **Player Choice:** Can spare or execute after defeat

#### Kurogane's Personal Lieutenant (Act 3 - Story-Connected)
- **Location:** Fortress inner sanctum
- **Health:** High mini-boss tier
- **Story Significance:** Directly responsible for carrying out Masahiro's execution
- **Attack Pattern:** Elite swordsmanship + magical enhancements
- **Teaches:** Mastery of all mechanics before final boss
- **Emotional Weight:** Confrontation reveals details about Masahiro's final moments

#### Other Mini-Bosses (Combat-Focused)
- Elite commanders teaching specific mechanics
- Bridge guards blocking progress
- Arena champions in optional challenges
- Each designed to test specific player skills

### Final Boss - Lord Kurogane

#### Phase 1: The Tyrant
- **Location:** Throne room - burning braziers, torn banners
- **Health:** 33% of total
- **Behavior:** Aggressive, heavy melee attacks
- **Attack Patterns:**
  - Sweeping horizontal slashes
  - Overhead power strikes
  - Guard-breaking attacks
- **Weakness:** Telegraphed attacks perfect for parry practice
- **Teaches:** Confidence in parry timing under pressure

#### Phase 2: The Despot
- **Health:** 33% of total
- **Behavior:** Activates magical enhancements
- **Attack Patterns:**
  - Magic-infused blade strikes (fire/lightning)
  - Summons elite guard reinforcements OR environmental traps
  - Faster combo chains
- **Weakness:** Magic attacks have charge-up, exploit cooldowns
- **Teaches:** Prioritization and crowd control

#### Phase 3: The Monster
- **Health:** Final 34%
- **Behavior:** Armor stripped, desperate and aggressive
- **Attack Patterns:**
  - Relentless melee-magic combinations
  - Unpredictable timing variations
  - Full pattern challenge requiring all learned skills
- **Weakness:** Exhaustion creates brief openings
- **Teaches:** Everything learned must be used

**Dialogue During Fight:**
- "Your mentor was weak. He should have obeyed."
- Changes slightly based on player choices and optional story completion
- Kaizen responds based on player's path (vengeance vs. honor)

**Outcomes:**
1. **Execution Ending:** Kill Kurogane - restore order through vengeance
2. **Mercy Ending (hard/optional):** Spare him - exile, reflects restraint
3. **Legacy Ending:** Dismantle rule completely - rebuild land, symbolic closure

**Rewards:**
- Depends on ending chosen
- Unlocks New Game+ mode
- Special weapons/abilities for subsequent playthroughs

---

## Level Design Philosophy

### Core Principles
- **Exploration rewards mastery:** Hidden lore rooms and side areas contain upgrades and story depth
- **Linear with optional branches:** Main path is clear, but exploration reveals secrets
- **Environmental storytelling:** Show Kurogane's tyranny through destroyed villages and suffering
- **Guided progression:** ~10 hour main story with optional content extending playtime
- **Choices have consequences:** Player decisions affect available content and endings

### Level Themes & Structure

**Total Game Length:** ~10 hours (main story) + 2-4 hours (optional content)

1. **Prologue: Mountain Dojo & Village (~1 hour)**
   - Visual Style: Peaceful mountain setting, traditional architecture
   - Mood: Calm training → Tragic violence
   - Tutorial integration, emotional hook established
   - Mini-boss: Foreman/Executioner General

2. **Act 2: The Tyrant's Reach (~4 hours)**
   - **Towns & Villages:** Destroyed, fearful citizens, signs of oppression
   - **Temples:** Corrupted, former places of peace now occupied
   - **Abandoned Villages:** Environmental storytelling of Kurogane's destruction
   - Visual Style: Gradual decay from civilization to ruin
   - Mood: Growing anger, determination, moral questions
   - Multiple mini-bosses, Elite Guards, story-connected boss
   - **Shops introduced:** Upgrade purchases, potion vendors, magic scrolls

3. **Act 3: The Tyrant's Domain (~3 hours)**
   - **Fortress Exterior:** Heavily fortified, oppressive architecture
   - **Capital City:** Contrasts opulence with suffering
   - **Fortress Interior:** Dark corridors, elite guards, magical defenses
   - Visual Style: Imposing, dark, showcases Kurogane's power
   - Mood: Final push, culmination of journey
   - Final story mini-boss, preparation for Kurogane

4. **Final Battle: Throne Room (~1 hour)**
   - Visual Style: Dramatic, burning braziers, torn banners
   - Three-phase boss battle with Kurogane
   - Mood: Intense, emotional payoff

5. **Epilogue (~1 hour)**
   - Varies by player choices
   - Shows impact of decisions
   - Character resolution

### Checkpoints & Respawn
- **Checkpoint Frequency:** Before/after major encounters, between level sections
- **Death Penalty:** Respawn at last checkpoint, keep ability upgrades, lose unsaved progress
- **Fast Travel:** Unlocks after Act 2, limited to major checkpoints
- **Optional Challenge Rooms:** Side areas with harder enemies, better rewards

### Interactive Elements
- **Townsfolk NPCs:** Provide lore, side quests, moral choices
- **Lore Rooms:** Hidden areas with scrolls revealing backstory
- **Shops:** Buy potions, magic scrolls, optional weapons, ability upgrades
- **Choice Moments:** Spare/kill enemies, help/ignore NPCs

---

## Art Style & Atmosphere

### Visual Style
- **Color Palette:** [Dark/Light? Vibrant/Muted? Specific colors?]
- **Art Direction:** [Pixel art? Hand-drawn? Realistic? Stylized?]
- **Animation Style:** [Smooth/Snappy? How many frames?]
- **Particle Effects:** [Heavy VFX or minimal?]

### Audio Direction
- **Music Style:** [Epic orchestral? Electronic? Ambient?]
- **Boss Music:** [How should boss themes feel?]
- **Sound Effects:** [Realistic? Arcade-style? Exaggerated?]

### Mood & Tone
[What emotional experience are you creating? Dark and serious? Light-hearted? Melancholic?]

---

## UI/UX Design

### HUD Elements
- **Health Display:** [Health bar? Numbers? Hearts?]
- **Ability Cooldowns:** [How to show what's available?]
- **Resource Counters:** [Currency, special items]
- **Mini-map:** [Yes/No? Style?]

### Menus
- **Main Menu:** [Options needed]
- **Pause Menu:** [What can player access?]
- **Inventory/Equipment:** [How complex?]
- **Skill Tree:** [Layout and style]

---

## Technical Notes

### Controls (Current Implementation)
- **A/D or Arrow Keys:** Move left/right
- **Space/W/Up Arrow:** Jump (press again in air for double jump)
- **ESC:** Pause/Exit

### Planned Controls (Kaizen Combat System)
- **Left Click / J:** Light Attack (fast combo)
- **Right Click / K:** Heavy Attack (guard break)
- **Q:** Spirit Strike (damage buff)
- **E:** Echo Step (dash)
- **R (Hold):** Blade Ward (defensive magic)
- **F:** Phantom Slash (multi-enemy)
- **Spacebar (Timed):** Parry - **CORE MECHANIC**
- **Shift (Hold):** Block/Guard
- **T:** Interact with NPCs/Objects
- **Tab:** Inventory/Equipment
- **M:** Map (when unlocked)

### Performance Targets
- **Target FPS:** 60
- **Resolution:** 1280x720 (scalable)
- **Platform:** Windows (expandable to other platforms)

---

## Development Roadmap

### Month 1 - Foundation (CURRENT)
- [x] Player movement
- [x] Physics system
- [x] Platform collision
- [x] Camera system
- [x] Double jump
- [x] Basic attack system (light + heavy with directional input)
- [x] Parry mechanic implementation

### Month 2 - Combat Foundation
- [x] Attack animations (light, heavy, combo)
- [x] Parry system with timing windows
- [x] Enemy AI (basic - patrol, chase, attack)
- [x] Health/damage system
- [x] Hit feedback (screen shake, knockback, invulnerability blink)
- [ ] Spirit Strike ability
- [ ] First mini-boss (Foreman/Executioner)

### Month 3 - Enhanced Combat & Magic
- [ ] Echo Step ability
- [ ] Blade Ward ability
- [ ] Elite enemy types
- [ ] Combo system refinement
- [ ] Magic visual effects
- [ ] Shop system implementation

### Month 4 - Level Design & Progression
- [ ] Prologue level complete
- [ ] Act 2 town/temple environments
- [ ] NPC interaction system
- [ ] Lore room discoveries
- [ ] Side quest framework
- [ ] Story-connected mini-boss (former ally)

### Month 5-6 - Advanced Systems
- [ ] Phantom Slash ability
- [ ] Soul Resonance (parry enhancement)
- [ ] Act 3 fortress environments
- [ ] Player choice system implementation
- [ ] Save/load system
- [ ] Inventory and equipment

### Month 7-8 - Boss & Content
- [ ] Lord Kurogane boss fight (3 phases)
- [ ] All mini-bosses implemented
- [ ] Complete level design (all acts)
- [ ] Optional allies (Ayame, Raizo) as unlockables
- [ ] Environmental puzzles

### Month 9 - Polish & Balance
- [ ] Combat balance tuning
- [ ] Difficulty scaling
- [ ] Animation polish
- [ ] Visual effects refinement
- [ ] Sound design integration
- [ ] UI/UX polish

### Month 10-11 - Multiple Endings & Testing
- [ ] Execution ending implementation
- [ ] Mercy ending implementation  
- [ ] Legacy ending implementation
- [ ] Choice consequence system fully integrated
- [ ] Playtesting and iteration
- [ ] Bug fixing

### Month 12 - Final Polish & Launch Prep
- [ ] Epilogue cinematics
- [ ] New Game+ mode
- [ ] Achievement system
- [ ] Final balance pass
- [ ] Performance optimization
- [ ] Marketing materials
- [ ] Launch build preparation

---

## References & Inspiration

### Games
- **Nine Sols:** Primary inspiration for mechanical and visual style
  - Parry-focused melee combat system
  - Fluid movement and responsive controls
  - 2D side-scrolling action platformer
  - Story-driven with emotional depth
  - Boss fights with multiple phases

### Art/Visual References
- Japanese/Asian-inspired fantasy aesthetic
- Traditional dojo and temple architecture
- Contrasting peaceful nature with oppressive fortress
- Magical enhancements visualized through particle effects

### Story/Narrative References
- **Themes:** Revenge vs. Honor, Mentorship, Tyranny, Choice and Consequence
- **Tone:** Dark but hopeful, emotional character journey
- **Inspiration:** Samurai/warrior narratives focusing on discipline and mastery

---

## Open Questions & Decisions

### Needs Design Decision
- [x] **Stamina system:** NO - Focus on skill-based parry timing instead
- [x] **Difficulty scaling:** Single difficulty with learnable patterns, optional harder New Game+
- [x] **Save system:** Checkpoint-based before/after major encounters
- [x] **Death penalty:** Respawn at last checkpoint, keep upgrades, lose unsaved progress
- [x] **Economy:** Single currency (Gold Coins) + rare materials for special upgrades
- [x] **Game length:** ~10 hours main story, +2-4 hours optional content
- [ ] Exact parry timing window (currently 6 frames - needs playtesting)
- [ ] Number of shops per act
- [ ] Exact mini-boss count (planned 6-8, needs finalization)

### Technical Considerations
- [ ] Maximum enemy count on screen
- [ ] Particle effect budget
- [ ] Animation frame count per action
- [ ] Sound effect channels needed

---

## Change Log

### 2026-01-13
- Created design document
- Implemented core movement and double-jump
- Camera system functional
- Basic platform collision working
- **Added complete Kaizen: Shadow of the Regent story outline:**
  - Full 3-act structure with prologue and epilogue
  - Main characters defined (Kaizen, Masahiro, Lord Kurogane)
  - All abilities specified (Spirit Strike, Echo Step, Blade Ward, Phantom Slash, Soul Resonance)
  - Mini-boss encounters detailed (6-8 total, 2 story-connected)
  - Final boss 3-phase fight designed
  - Multiple ending system outlined
  - Player choice and consequence systems defined
  - ~10 hour gameplay structure established
- **Implemented attack system:**
  - Light attack (J key): Fast, 10 damage, 0.4s cooldown
  - Heavy attack (K key): Slow, 20 damage, 0.7s cooldown
  - Visual hitbox feedback (semi-transparent red)
  - Attack lifetime and fade-out effects
  - Direction-aware hitbox positioning

---

**Note:** This document is a living reference. Update it as your vision evolves and new ideas emerge. Keep it as detailed or minimal as you need—it's your guide!
