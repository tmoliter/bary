// #include "EventEditor.h"

// EventEditor::EventEditor(RealThing *p) : 
// parent(p), 
// input(""),
// eventName(""),
// collidable(make_pair("",nullptr)),
// previewPhrase(nullptr),
// soundName(""),
// collidableType(CollidableType::interactable), 
// eventType(EventType::simpleMessage),
// boxState(BoxEditState::resize) {
//     parent->showInteractableLines();
//     text = new Text(Point(settings.LETTER_WIDTH * 2, settings.LETTER_HEIGHT * 4), "");
//     UIRenderer::addText(text);
//     changeState(EventEditState::selectCollidableType);
// };

// EventEditor::~EventEditor() {
//     UIRenderer::removeText(text);
// };

// // NEED CANCEL ACTIONS

// int EventEditor::changeState(EventEditState nextState) {
//     switch(nextState) {
//         int totalInteractables, totalTriggers;
//         case EventEditState::selectCollidableType:
//             totalInteractables = parent->interactables.size();
//             totalTriggers = parent->triggers.size();
//             collidable = make_pair("",nullptr);
//             if (totalInteractables < 1 && totalTriggers < 1)
//                 return 1;
//             if (totalInteractables < 1) {
//                 collidableType = CollidableType::trigger;
//                 changeState(EventEditState::chooseCollidable);
//                 return 0;
//             }
//             if (totalTriggers < 1) {
//                 collidableType = CollidableType::interactable;
//                 changeState(EventEditState::chooseCollidable);
//                 return 0;
//             }
//             gameState = GameState::FieldFree;
//             editState = nextState;
//             updateDisplay();
//             return 0;
//         case EventEditState::chooseCollidable:
//             totalInteractables = parent->interactables.size();
//             totalTriggers = parent->triggers.size();
//             collidable = make_pair("",nullptr);
//             if (collidableType == CollidableType::interactable && totalInteractables == 1) {
//                 collidable = *parent->interactables.begin();
//                 changeState(EventEditState::selectEventType);
//                 return 0;
//             }
//             if (collidableType == CollidableType::trigger && totalTriggers == 1) {
//                 collidable = *parent->triggers.begin();
//                 changeState(EventEditState::selectEventType);
//                 return 0;
//             }
//             if (collidableType == CollidableType::interactable)
//                 availableCollidables = parent->findAndShowInteractableLines(input);
//             else
//                 availableCollidables = parent->findAndShowTriggerLines(input);
//             gameState = GameState::TextInput;
//             editState = nextState;
//             updateDisplay();
//             return 0;
//         case EventEditState::selectEventType:
//             if (eventMap::eventNameToGenerator.size() < 1) {
//                 eventType = EventType::simpleMessage;
//                 changeState(EventEditState::enterMessage);
//                 return 0;
//             }
//             gameState = GameState::FieldFree;
//             editState = nextState;
//             updateDisplay();
//             return 0;
//         case EventEditState::choosePredefined:
//             gameState = GameState::TextInput;
//             editState = nextState;
//             updateDisplay();
//             return 0;
//         case EventEditState::predefinedSuccess:
//             gameState = GameState::FieldFree;
//             editState = nextState;
//             updateDisplay();
//             return 0;
//         case EventEditState::enterMessage:
//             gameState = GameState::TextInput;
//             editState = nextState;
//             text->lineLength = 30;
//             updateDisplay();
//             return 0;
//         case EventEditState::enterSoundName:
//             gameState = GameState::TextInput;
//             editState = nextState;
//             updateDisplay();
//             return 0;
//         case EventEditState::editBox:
//             gameState = GameState::FieldFree;
//             editState = nextState;
//             updateDisplay();
//             return 0;
//         case EventEditState::messageSuccess:
//             gameState = GameState::FieldFree;
//             editState = nextState;
//             updateDisplay();
//             return 0;
//         default:
//             return 0;
//     }
//     return 0;
// }


// void EventEditor::updateDisplay() {
//     string displayText, prefix;
//     int maxLine, size, magnitude, remainder;
//     text->clearText();
//     switch (editState) {
//         case EventEditState::selectCollidableType:
//             displayText = "Select collidable type:` " + 
//                 string(collidableType == CollidableType::interactable ? "interactable" : "trigger");
//             break;
//         case EventEditState::chooseCollidable:
//             displayText =  "Enter collidable name` " + input + "`  List:";
//             for (auto name : availableCollidables) {
//                 displayText = displayText + "`  " + eventMap::namePlusEvent(parent->name, name, collidableType);
//             }
//             break;
//         case EventEditState::selectEventType:
//             displayText =  "Select event type for collidable " + collidable.first + 
//                 string(eventType == EventType::predefined ? + ":` predefined" : ":` simple message" );
//             break;
//         case EventEditState::choosePredefined:
//             displayText =  "Enter event name from list for collidable '" + 
//                 collidable.first + "'` "
//                 + input + "`  List:";
//             for (auto const& [name, fnptr] : eventMap::eventNameToGenerator)
//                 if (input.size() < 1 || name.rfind(input, 0) == 0)
//                     displayText = displayText + "`  " + name;
//             break;
//         case EventEditState::predefinedSuccess:
//             displayText =  "Successfully added event '" + eventName + "' to collidable '" + collidable.first + "'!";
//             break;
//         case EventEditState::enterMessage:
//             displayText =  "Enter message for collidable '" + collidable.first + "':` " + input;
//             break;
//         case EventEditState::enterSoundName:
//             displayText =  "Enter sound path or press return to continue for collidable '" + collidable.first + "':` " + input;
//             break;
//         case EventEditState::editBox:
//             switch (boxState) {
//                 case BoxEditState::resize:
//                     displayText = "Resize phrase box";
//                     break;
//                 case BoxEditState::padding:
//                     displayText = "Adjust phrase padding";
//                     break;
//                 case BoxEditState::move:
//                     displayText = "Move phrase box";
//                     break;
//             }
//             break;
//         case EventEditState::messageSuccess:
//             displayText =  "Successfully added Simple Message to collidable '" + collidable.first + "'!";
//             break;
//         default:
//             break;
//     }
//     text->setText(displayText);
// }


// int EventEditor::routeInput(KeyPresses keysDown) {
//     switch (editState) {
//     case EventEditState::selectCollidableType:
//         selectCollidableType(keysDown);
//         break;
//     case EventEditState::chooseCollidable:
//         chooseCollidable(keysDown);
//         break;
//     case EventEditState::selectEventType:
//         selectEventType(keysDown);
//         break;
//     case EventEditState::choosePredefined:
//         choosePredefined(keysDown);
//         break;
//     case EventEditState::enterSoundName:
//         enterSoundName(keysDown);
//         break;
//     case EventEditState::enterMessage:
//         enterMessage(keysDown);
//         break;
//     case EventEditState::editBox:
//         editBox(keysDown);
//         break;
//     case EventEditState::predefinedSuccess:
//     case EventEditState::messageSuccess:
//         if (keysDown.ok)
//             return 1;
//     default:
//         break;
//     }
//     return 0;
// }

// void EventEditor::selectCollidableType (KeyPresses keysDown) {
//     if (keysDown.ok) {
//         changeState(EventEditState::chooseCollidable);
//     }
//     if( keysDown.up || keysDown.debug_up) {
//         collidableType = CollidableType::interactable;
//         parent->showInteractableLines();
//         updateDisplay();
//     }
//     if (keysDown.down || keysDown.debug_down) {
//         collidableType = CollidableType::trigger;
//         parent->showTriggerLines();
//         updateDisplay();
//     }
// }

// void EventEditor::chooseCollidable (KeyPresses keysDown) {
//     if (keysDown.textInput){
//         input.push_back(keysDown.textInput);
//         if (collidableType == CollidableType::interactable)
//             availableCollidables = parent->findAndShowInteractableLines(input);
//         else
//             availableCollidables = parent->findAndShowTriggerLines(input);
//         updateDisplay();
//     }
//     if (keysDown.del && input.length() > 0){
//         input.pop_back();
//         if (collidableType == CollidableType::interactable)
//             availableCollidables = parent->findAndShowInteractableLines(input);
//         else
//             availableCollidables = parent->findAndShowTriggerLines(input);
//         updateDisplay();
//     }
//     if (keysDown.start && availableCollidables.size() == 1) {
//         if (collidableType == CollidableType::interactable)
//             collidable = make_pair(availableCollidables.back(), parent->interactables[availableCollidables.back()]);
//         else
//             collidable = make_pair(availableCollidables.back(), parent->triggers[availableCollidables.back()]);
//         changeState(EventEditState::selectEventType);
//     }
// }


// void EventEditor::selectEventType (KeyPresses keysDown) {
//     if (keysDown.ok) {
//         switch (eventType) {
//             case EventType::simpleMessage:
//                 changeState(EventEditState::enterSoundName);
//                 return;
//             case EventType::predefined:
//                 changeState(EventEditState::choosePredefined);
//                 return;
//         }
//     }
//     if( keysDown.up || keysDown.debug_up) {
//         switch (eventType) {
//             case EventType::simpleMessage:
//             case EventType::predefined:
//                 eventType = EventType::simpleMessage;
//                 input.clear();
//                 updateDisplay();
//                 break;
//         }
//     }
//     if (keysDown.down || keysDown.debug_down) {
//         switch (eventType) { 
//             case EventType::simpleMessage:
//             case EventType::predefined:
//                 eventType = EventType::predefined;
//                 updateDisplay();
//                 break;
//         }
//     }
// }

// void EventEditor::choosePredefined (KeyPresses keysDown) {
//     if (keysDown.textInput){
//         input.push_back(keysDown.textInput);
//         updateDisplay();
//     }
//     if (keysDown.del && input.length() > 0){
//         input.pop_back();
//         updateDisplay();
//     }
//     if (keysDown.start) {
//         vector<string> availableEvents;
//         for (auto const& [name, fnptr] : eventMap::eventNameToGenerator)
//             if (input.size() < 1 || name.rfind(input, 0) == 0)
//                 availableEvents.push_back(name);
//         if(availableEvents.size() == 1) {
//             eventName = availableEvents.back();
//             eventMap::attachEvent(eventName, parent->name, collidable.first, collidableType);
//             changeState(EventEditState::predefinedSuccess);
//         }
//     }
// }

// void EventEditor::enterSoundName (KeyPresses keysDown) {
//     if (keysDown.textInput){
//         input.push_back(keysDown.textInput);
//         updateDisplay();
//     }
//     if (keysDown.del && input.length() > 0){
//         input.pop_back();
//         updateDisplay();
//     }
//     if (keysDown.start) {
//         soundName = input;
//         input.clear();
//         changeState(EventEditState::enterMessage);
//     }
// }

// void EventEditor::enterMessage (KeyPresses keysDown) {
//     if (keysDown.textInput){
//         input.push_back(keysDown.textInput);
//         updateDisplay();
//     }
//     if (keysDown.del && input.length() > 0){
//         input.pop_back();
//         updateDisplay();
//     }
//     if (keysDown.start && input.size() > 0) {
//         previewPhrase = new Phrase(Point(120,320), Point(400, 90), ScrollType::preview, input);
//         UIRenderer::addPhrase(previewPhrase);
//         changeState(EventEditState::editBox);
//         text->resetLineLength();
//     }
// }

// void EventEditor::editBox(KeyPresses keysDown) {
//     bool changed = false;
//     if (boxState == BoxEditState::resize) {
//         if (keysDown.cancel) {
//             input = previewPhrase->text;
//             UIRenderer::removePhrase(previewPhrase);
//             changeState(EventEditState::enterMessage);
//             return;
//         }
//         if (keysDown.ok) {
//             boxState = BoxEditState::padding;
//             updateDisplay();
//             return;
//         }
//         SDL_Rect &box = previewPhrase->getBox();
//         if((keysDown.up || keysDown.debug_up) && box.h >= settings.LETTER_HEIGHT + 2) {
//             box.h--;
//             changed = true;
//         }
//         if ((keysDown.down || keysDown.debug_down)) {
//             box.h++;
//             changed = true;
//         }
//         if ((keysDown.left || keysDown.debug_left) && box.w >= settings.LETTER_WIDTH + 2) {
//             box.w--;
//             changed = true;
//         }
//         if ((keysDown.right || keysDown.debug_right)) {
//             box.w++;
//             changed = true;
//         }
//     }

//     if (boxState == BoxEditState::padding) {
//         if (keysDown.cancel) {
//             boxState = BoxEditState::resize;
//             updateDisplay();
//             return;
//         }
//         if (keysDown.ok) {
//             boxState = BoxEditState::move;
//             updateDisplay();
//             return;
//         }
//         DirectionMap dM;
//         if((keysDown.up || keysDown.debug_up)) {
//             dM.up = true;
//             changed = true;
//         }
//         if ((keysDown.down || keysDown.debug_down)) {
//             dM.down = true;
//             changed = true;
//         }
//         if ((keysDown.left || keysDown.debug_left)) {
//             dM.left = true;
//             changed = true;
//         }
//         if ((keysDown.right || keysDown.debug_right)) {
//             dM.right = true;
//             changed = true;
//         }
//         if (changed) {
//             previewPhrase->setGridLimits(dM);
//             return;
//         }
//     }

//     if (boxState == BoxEditState::move) {
//         if (keysDown.cancel) {
//             boxState = BoxEditState::padding;
//             updateDisplay();
//             return;
//         }
//         if (keysDown.ok) {
//             Phrase *newPhrase = new Phrase(*previewPhrase);
//             newPhrase->scrollType = ScrollType::allButLast;
//             UIRenderer::removePhrase(previewPhrase);
//             delete previewPhrase;
//             previewPhrase = nullptr;

//             // TODO Deal with multi-message events
//             EventCollidable *ec = collidable.second;
//             if (ec->event != nullptr) {
//                 SimpleMessage* sm = dynamic_cast<SimpleMessage*>(ec->event);
//                 if (sm)
//                     sm->addPhrase(newPhrase);
//                 else
//                     delete ec->event;
//             } else
//                 ec->event = new SimpleMessage(newPhrase, soundName);
//             changeState(EventEditState::messageSuccess);
//             return;
//         }
//         if((keysDown.up || keysDown.debug_up)) {
//             previewPhrase->box.y--;
//             changed = true;
//         }
//         if ((keysDown.down || keysDown.debug_down)) {
//             previewPhrase->box.y++;
//             changed = true;
//         }
//         if ((keysDown.left || keysDown.debug_left)) {
//             previewPhrase->box.x--;
//             changed = true;
//         }
//         if ((keysDown.right || keysDown.debug_right)) {
//             previewPhrase->box.x++;
//             changed = true;
//         }
//     }
    
//     if ((keysDown.menu1 && previewPhrase->phraseScale > 1)) {
//         previewPhrase->phraseScale /= 2;
//         changed = true;
//     }
//     if ((keysDown.menu2 && previewPhrase->phraseScale < 4)) {
//         previewPhrase->phraseScale *= 2;
//         changed = true;
//     }
//     if (changed) {
//         previewPhrase->reset();
//     }
// }
