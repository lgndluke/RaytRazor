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

    if (parentName.empty()) {
        auto rootContainer = new Widget(mContainer);
        rootContainer->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, 5));

        auto label = new Custom_Label(rootContainer, nodeName, "sans-bold");
        label->setFontSize(Basis_Root_Font);
        mNodeMap[nodeName] = rootContainer;
    } else if (mNodeMap.find(parentName) != mNodeMap.end()) {
        auto parentWidget = mNodeMap[parentName];

        auto childContainer = new Widget(parentWidget);
        childContainer->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum, 0, 0));
        auto label = new Custom_Label(childContainer, "|-------> " + nodeName, "sans");
        label->setFontSize(Basis_Child_Font);
        label->setCallback([this, nodeName]() {
            printf("Node name: %s\n", nodeName.c_str());
            //todo mAttributes->showAttributesOfComponent(Main_Scene::getComponents());
        });

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
