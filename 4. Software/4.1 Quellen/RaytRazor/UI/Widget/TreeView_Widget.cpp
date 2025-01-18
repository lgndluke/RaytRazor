#include "../Scenes/Main/Main_Scene.h"
#include "TreeView_Widget.h"


using namespace std;

int Basis_Root_Font = 35;
int Basis_Child_Font = 25;

TreeView_Widget::TreeView_Widget(Widget* parent, ComponentAttributes_Widget* attributesWidget)
    : Widget(parent), mAttributes(attributesWidget) {
    auto scrollPanel = new VScrollPanel(this);
    scrollPanel->setFixedSize({parent->width(), parent->height()});

    // Container für Baumknoten
    mContainer = new Widget(scrollPanel);
    mContainer->setLayout(new BoxLayout(
        Orientation::Vertical, Alignment::Minimum, 5, 5
    ));
}

void TreeView_Widget::addNode(const std::string& nodeName, const std::string& parentName) {
    if (mNodeMap.find(nodeName) != mNodeMap.end()) {
        return;
    }

    if (parentName.empty()) {
        // Erstellen eines Root-Knotens
        auto rootContainer = new Widget(mContainer);
        rootContainer->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, 5));

        auto label = new Custom_Label(rootContainer, nodeName, "sans-bold");
        label->setFontSize(Basis_Root_Font);
        mNodeMap[nodeName] = rootContainer; // Speichern des Knotens in der Map
    } else if (mNodeMap.find(parentName) != mNodeMap.end()) {
        // Erstellen eines Child-Knotens unter dem angegebenen Parent
        auto parentWidget = mNodeMap[parentName];

        auto childContainer = new Widget(parentWidget);
        childContainer->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum, 0, 0));

        auto label = new Custom_Label(childContainer, "|-------> " + nodeName, "sans");
        label->setFontSize(Basis_Child_Font);

        // Callback für das Hervorheben des angeklickten Labels
        label->setCallback([this, label, nodeName]() {
            printf("Node name: %s\n", nodeName.c_str());

            // Reset the previous label's color if it exists
            if (mCurrentSelectedLabel) {
                mCurrentSelectedLabel->setColor(Color(255, 255, 255, 255)); // Standardfarbe (z. B. Weiß)
            }

            // Highlight the clicked label
            label->setColor(Color(255, 0, 0, 255)); // Highlight-Farbe (z. B. Rot)

            // Update the current selected label
            mCurrentSelectedLabel = label;

            // Update the component attributes
            for (auto componets : Main_Scene::getComponents()) {
                if (nodeName == componets.second->get_name()) {
                    mAttributes->updateFromComponent(componets.second);
                }
            }
        });

        // Speichern des neuen Child-Knotens in der Map
        mNodeMap[nodeName] = childContainer;
    }
}

void TreeView_Widget::clear() {
    while (!mContainer->children().empty()) {
        Widget* child = mContainer->children().back();
        mContainer->removeChild(child);
    }
    mNodeMap.clear();
}
