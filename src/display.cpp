#include "display.h"
#include <iostream>
#include <string>

std::string colorizeNumber(double value) {
  std::string num = "";
  if (value < 0) {
      num = Color::red + std::to_string(value) + Color::reset;
  } else if (value > 0) {
      num =  Color::green + std::to_string(value) + Color::reset;
  } else {
      num = std::to_string(value);
  }
  return num;
};

std::string bar(double value, std::string color) {
  int blocks = value / 5;
  std::string bar = color;
  bar += "[";
  for (int i = 0; i < 20; ++i)
      bar += (i < blocks) ? "#" : " ";
  bar += "]";
  bar += Color::reset;
  return bar;
}

void Display::displayProgress(const Progress& progress) const {
    std::cout << "theory:    " << bar(progress.theory, Color::green) << " " << progress.theory << '\n'
              << "practice:  " << bar(progress.practice, Color::cyan) << " " << progress.practice << '\n'
              << "grade:     " << bar(progress.grade, Color::yellow) << " " << progress.grade << '\n';
}

void Display::displayDayStudy(const Day& day) const {
  auto available_actions = day.getActions();
  int study_cnt = 0;
  for (int i = 0; i < available_actions.size(); ++i) {
    auto action = available_actions[i];
    if (action->isStudy() && !action->isAlwaysAvailable()) {
      ++study_cnt;
      std::cout << "- - - - - - - - - - - - - - - - - -\n";
      std::cout << i + 1 << " : ";
      displayAction(action, day.getTime());
    }
  }
  if (study_cnt == 0) {
    std::cout << "It's done for today, go to sleep\n";
  }
}

void Display::displayDayHomework(const Day& day) const {
  auto available_actions = day.getActions();
  if (available_actions.size() == 0) {
    std::cout << "It's done for today, go to sleep\n";
  }

  for (int i = 0; i < available_actions.size(); ++i) {
    auto action = available_actions[i];
    if (action->isStudy() && action->isAlwaysAvailable()) {
      std::cout << "- - - - - - - - - - - - - - - - - -\n";
      std::cout << i + 1 << " : ";
      displayAction(action, day.getTime());
    }
  }
}

void Display::displayDayChillWork(const Day& day) const {
  auto available_actions = day.getActions();
  if (available_actions.size() == 0) {
    std::cout << "It's done for today, go to sleep\n";
  }

  for (int i = 0; i < available_actions.size(); ++i) {
    auto action = available_actions[i];
    if (!action->isStudy()) {
      std::cout << Color::cyan << "======================" << Color::reset << "\n";
      std::cout << i + 1 << " : ";
      displayAction(action, day.getTime());
    }
  }
}

void Display::displayAttributes(const PlayerAttributes& player_attributes) const {
  std::cout << Color::yellow << "energy: " << Color::reset << player_attributes.energy << '\n'
            << "loneliness: " << player_attributes.loneliness << '\n'
            << Color::green << "ability to study: " << Color::reset << player_attributes.studying_capacity << '\n';
}

void Display::displayItemInformation(const Item& item) const {
    std::cout << Color::cyan << "╔═════════════════════ ITEM ════════════════════╗" << Color::reset << '\n';
    std::cout << Color::yellow << Color::bold << "Name: "
              << Color::green << item.getName() << Color::reset << '\n';
    std::cout << Color::magenta << "Effects:\n" << Color::reset;
    displayAttributes(item.getPlayerAffect());
    std::cout << Color::yellow << "Cost: "
              << Color::bold << item.getCost() << " rubles" << Color::reset << '\n';
    std::cout << Color::cyan << "╚═══════════════════════════════════════════════╝" << Color::reset << '\n';
}


void Display::displayInvetory(const Inventory& inventory) const {
    std::cout << Color::yellow << "╔════ Current Inventory ════╗" << Color::reset << '\n';
    std::cout << Color::yellow << "Balance: " << inventory.getMoney() << Color::reset << '\n';
    for (auto& item : inventory.getCurrentInvetory()) {
        std::cout << Color::green << item.getName() << Color::reset
                  << " : " << inventory.getCounts()[item.getName()] << '\n';
    }
    std::cout << Color::yellow << "╚═══════════════════════════╝" << Color::reset << '\n';
}

void Display::displayShop(const Shop& shop, const Player& player) const {
    std::cout << Color::cyan << "╔═══ Welcome to Hach's tent! ═══╗\n" << Color::reset;

    if (shop.isSale()) {
        std::cout << Color::magenta << Color::bold << "✨ Today is SALE!! You can buy anything without limit! ✨\n" << Color::reset;
    }

    std::cout << Color::yellow << "Your current balance: " << Color::bold
              << player.getInventory().getMoney() << " rubles" << Color::reset << "\n";

    std::cout << Color::cyan << "--------------------------------------" << Color::reset << "\n";

    for (const auto& item : shop.getCurrentItems()) {
        std::cout << Color::green << Color::bold << item.getName() << Color::reset
                  << "  -  " << Color::yellow << item.getCost() << " rub" << Color::reset
                  << "  |  " << Color::cyan << "Left: " << shop.getCounts()[item.getName()] << Color::reset << '\n';

        std::cout << Color::magenta << "  Effects:\n" << Color::reset;
        displayAttributes(item.getPlayerAffect());

        std::cout << Color::cyan << "--------------------------------------" << Color::reset << "\n";
    }
    std::cout << Color::cyan << "╚════════════════════════════════════╝" << Color::reset << "\n";
}

void Display::displayAction(const Action* action, Time time) const {
  std::cout << Color::cyan << "╔═══════════ ACTION ═══════════╗\n" << Color::reset;

  if (action->isStudy()) {
    std::cout << Color::yellow << Color::bold << action->getName() << Color::reset
              << " on "
              << Color::green << action->getSubject()->name << Color::reset << '\n';

    if (!action->isAlwaysAvailable()) {
      std::cout << Color::cyan << "Begins at " << Color::magenta << action->getBeginningTime(time).getTime() << Color::reset << '\n';
      std::cout << Color::cyan << "Lasts    : " << Color::magenta << action->getDuration(time) << " minutes" << Color::reset << '\n';
    } else {
      std::cout << Color::green << "(Available anytime)\n" << Color::reset;
    }

    std::cout << Color::magenta << "Has following effect on you";
    if (action->isAlwaysAvailable()) std::cout << Color::cyan << " (per minute)" << Color::reset;
    std::cout << Color::magenta << ":\n" << Color::reset;

    PlayerAttributes player_affect;
    Progress subject_affect;
    if (!action->isAlwaysAvailable()) {
      player_affect = action->getPlayerAffect() * action->getDuration(time);
      subject_affect = action->getAddProgress() * action->getDuration(time);
    } else {
      player_affect = action->getPlayerAffect();
      subject_affect = action->getAddProgress();
    }
    displayAttributes(player_affect);

    std::cout << Color::magenta << "Has following effect on subject";
    if (action->isAlwaysAvailable()) std::cout << Color::cyan << " (per minute)" << Color::reset;
    std::cout << Color::magenta << ":\n" << Color::reset;
    displayProgress(subject_affect);

  } else {
    std::cout << Color::yellow << Color::bold << action->getName() << Color::reset << '\n';
    std::cout << Color::magenta << "Has following effect on you";
    if (action->isAlwaysAvailable()) std::cout << Color::cyan << " (per minute)" << Color::reset;
    std::cout << Color::magenta << ":\n" << Color::reset;
    displayAttributes(action->getPlayerAffect());
  }

  std::cout << Color::cyan << "╚══════════════════════════════╝\n" << Color::reset;
}

void Display::displayRoomCoefficients(const Room& room) const {
  std::cout << Color::cyan << "╔═══ Room Coefficients ═══╗" << Color::reset << '\n';
  std::cout << Color::yellow << "Location: " << Color::green << room.name << Color::reset << '\n';
  displayAttributes(room.coefficients);
  std::cout << Color::cyan << "╚═════════════════════════╝" << Color::reset << '\n';
}

void Display::displayTime(const Time& time) const {
  std::cout << Color::cyan << "═════════════════════════════\n" << Color::reset;
  std::cout << Color::yellow << "Days lived: " << Color::green << time.day << Color::reset << '\n';
  std::cout << Color::yellow << "Current time: " << Color::magenta << time.getTime() << Color::reset << '\n';
  std::cout << Color::cyan << "═════════════════════════════" << Color::reset << '\n';
}

void Display::displayIntro() const {
  std::cout << Color::cyan << "╔════════════════════════════════════════════╗\n" << Color::reset
            << Color::yellow << Color::bold
            << "You entered Dolgoprudny town...\n"
            << Color::reset
            << Color::magenta
            << "There are only two ways to leave it --\n"
            << "   to graduate MIPT or to be kicked out.\n\n"
            << Color::reset
            << Color::green
            << "It's only you to decide:\n"
            << "  - Sleep all day, or\n"
            << "  - Visit all lectures and seminars.\n"
            << Color::reset
            << Color::yellow << "To graduate, you need at least 80 points\n"
            << "of progress in each class.\n"
            << Color::reset
            << Color::bold << "Good luck, warrior!\n"
            << Color::reset
            << Color::cyan << "╚════════════════════════════════════════════╝\n\n" << Color::reset;
}

void Display::displayHelp() const {
  std::cout
    << Color::cyan << "╔══════════════ HELP ══════════════╗\n" << Color::reset
    << Color::yellow << "[SC]"     << Color::reset << " -- See classes (upcoming actions for today).\n"
    << Color::yellow << "[SH]"     << Color::reset << " -- See homework (all available homework).\n"
    << Color::yellow << "[SCJ]"    << Color::reset << " -- See chill and job (available chills/jobs).\n"
    << Color::yellow << "[A]"      << Color::reset << " -- Your attributes.\n"
    << Color::yellow << "[R]"      << Color::reset << " -- Your current room.\n"
    << Color::yellow << "[CL]"     << Color::reset << " -- Change location.\n"
    << Color::yellow << "[T]"      << Color::reset << " -- Current time.\n"
    << Color::yellow << "[P]"      << Color::reset << " -- Progress in each class.\n"
    << Color::yellow << "[I]"      << Color::reset << " -- Inventory.\n"
    << Color::yellow << "[S]"      << Color::reset << " -- Shop (all goods).\n"
    << Color::yellow << "[BI]"     << Color::reset << " -- Buy item (then enter name).\n"
    << Color::yellow << "[SI]"     << Color::reset << " -- Sell item (then enter name).\n"
    << Color::yellow << "[AI]"     << Color::reset << " -- Apply item (then enter name).\n"
    << Color::yellow << "[H]"      << Color::reset << " -- See this help again.\n"
    << Color::yellow << "[E]"      << Color::reset << " -- End the game ("
        << Color::red << "not recommended" << Color::reset << ").\n"
    << Color::cyan << "╚══════════════════════════════════╝\n" << Color::reset;
}

void Display::displayRoom(const Room& room) const {
    std::cout << Color::yellow << "Now you are in: "
              << Color::green << Color::bold << room.name << Color::reset << '\n';
}
