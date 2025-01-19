#include "../Scenes/Main/Main_Scene.h"
#include "TreeView_Widget.h"


using namespace std;

int Basis_Root_Font = 35;
int Basis_Child_Font = 25;

TreeView_Widget::TreeView_Widget(Widget* parent, ComponentAttributes_Widget* attributesWidget)
    : Widget(parent), mAttributes(attributesWidget) {

    mScrollPanel = new VScrollPanel(this);
    mScrollPanel->setFixedSize({parent->width(), parent->height()});

    mContainer = new Widget(mScrollPanel);
    mContainer->setLayout(new BoxLayout(
        Orientation::Vertical, Alignment::Minimum, 5, 5
    ));

    mScrollPanel->setScroll(0.0f);
}

void TreeView_Widget::addParent(const std::string& parentName) {
    if (mNodeMap.find(parentName) != mNodeMap.end()) {
        return;
    }
    auto parentContainer = new Widget(mContainer);
    parentContainer->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, 5));

    auto label = new Custom_Label(parentContainer, parentName, "sans-bold");
    label->setFontSize(Basis_Root_Font);

    mNodeMap[parentName] = parentContainer;
}

void TreeView_Widget::addNode(const std::shared_ptr<Base_Component>& nodeName, const std::string& parentName) {
    if (mTrackedObjects.find(nodeName->get_uuid()) != mTrackedObjects.end()) {
        return;
    }

    if (parentName.empty()) {
        // Erstellen eines Root-Knotens
        auto rootContainer = new Widget(mContainer);
        rootContainer->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, 5));

        auto label = new Custom_Label(rootContainer, nodeName->get_name(), "sans-bold");
        label->setFontSize(Basis_Root_Font);
        mNodeMap[nodeName->get_name()] = rootContainer; // Speichern des Knotens in der Map
    } else if (mNodeMap.find(parentName) != mNodeMap.end()) {
        // Erstellen eines Child-Knotens unter dem angegebenen Parent
        auto parentWidget = mNodeMap[parentName];

        auto childContainer = new Widget(parentWidget);
        childContainer->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum, 0, 0));

        auto label = new Custom_Label(childContainer, "|-------> " + nodeName->get_name(), "sans");
        label->setFontSize(Basis_Child_Font);

        label->setCallback([this, label, nodeName]() {
            printf("Node name: %s\n", nodeName->get_name().c_str());
            if (mCurrentSelectedLabel) {
                mCurrentSelectedLabel->setColor(Color(255, 255, 255, 255)); // Standardfarbe (z. B. Weiß)
            }
            label->setColor(Color(255, 0, 0, 255));

            mCurrentSelectedLabel = label;

            for (const auto& componets : Main_Scene::getComponents()) {
                if (nodeName->get_uuid() == componets.second->get_uuid()) {
                    mAttributes->updateFromComponent(componets.second);
                }
            }
        });

        // Speichern des neuen Child-Knotens in der Map
        mNodeMap[nodeName->get_name()] = childContainer;
    }
}

void TreeView_Widget::clear() {
    while (!mContainer->children().empty()) {
        Widget* child = mContainer->children().back();
        mContainer->removeChild(child);
    }
    mNodeMap.clear();
}
