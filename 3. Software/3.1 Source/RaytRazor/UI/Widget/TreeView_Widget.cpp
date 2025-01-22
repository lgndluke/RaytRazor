#include "../Scenes/Main/Main_Scene.h"
#include "TreeView_Widget.h"


using namespace std;

int Basis_Root_Font = 35;
int Basis_Child_Font = 25;

TreeView_Widget::TreeView_Widget(Widget* parent, ComponentAttributes_Widget* attributesWidget)
    : Widget(parent), mAttributes(attributesWidget) {

    int scrollPanelHeight = std::max(100, parent->height() - (Basis_Child_Font * 2));
    mScrollPanel = new VScrollPanel(this);
    mScrollPanel->setFixedSize(Eigen::Vector2i(parent->width(), scrollPanelHeight));

    mContainer = new Widget(mScrollPanel);
    mContainer->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 5, 5));
    mContainer->setFixedSize(Vector2i(parent->width(), scrollPanelHeight * 2));

    mScrollPanel->performLayout(screen()->nvgContext());
    mContainer->performLayout(screen()->nvgContext());

    // Debugging
    std::cout << "TreeView_Widget initialized." << std::endl;
    std::cout << "ScrollPanel size: " << mScrollPanel->size().x() << ", " << mScrollPanel->size().y() << std::endl;
    std::cout << "Container size: " << mContainer->size().x() << ", " << mContainer->size().y() << std::endl;
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
    mContainer->performLayout(screen()->nvgContext());
}

void TreeView_Widget::addNode(const std::shared_ptr<Base_Component>& nodeName, const std::string& parentName) {
    try
    {
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
        } else if (mNodeMap.find(parentName) != mNodeMap.end())
        {
            // Erstellen eines Child-Knotens unter dem angegebenen Parent
            auto parentWidget = mNodeMap[parentName];

            auto childContainer = new Widget(parentWidget);
            childContainer->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum, 0, 0));

            auto label = new Custom_Label(childContainer, "|-------> " + nodeName->get_name(), "sans");
            label->setFontSize(Basis_Child_Font);

            label->setColor(Color(255, 255, 255, 255));

            if (mCurrentSelectedLabel && mCurrentSelectedLabel->first == nodeName->get_uuid()){
                label->setColor(Color(255, 0, 0, 255));
            }

            label->setCallback([this, label, nodeName]() {
                try {
                    printf("Node name: %s\n", nodeName->get_name().c_str());
                    if (mCurrentSelectedLabel) {
                        mCurrentSelectedLabel->second->setColor(Color(255, 255, 255, 255)); // Standardfarbe (z. B. Weiß)
                    }
                    label->setColor(Color(255, 0, 0, 255));

                    mCurrentSelectedLabel = new std::pair<boost::uuids::uuid, Custom_Label*>(nodeName->get_uuid(), label);

                    for (const auto& componets : Main_Scene::getComponents()) {
                        if (nodeName->get_uuid() == componets.second->get_uuid()) {
                            mAttributes->updateFromComponent(componets.second);
                        }
                    }
                } catch (const std::exception& e)
                {
                    Logger::log(MessageType::SEVERE, e.what());
                }
            });

            // Speichern des neuen Child-Knotens in der Map
            mNodeMap[nodeName->get_name()] = childContainer;
            mContainer->setSize(Vector2i(mScrollPanel->width(), mContainer->children().size() * label->height()));
            mContainer->performLayout(screen()->nvgContext());
            mScrollPanel->performLayout(screen()->nvgContext());
            printf("");
        }
    } catch (const std::runtime_error& e)
    {
        Logger::log(MessageType::SEVERE, e.what());
    }
}

void TreeView_Widget::clear() {
    while (!mContainer->children().empty()) {
        Widget* child = mContainer->children().back();
        mContainer->removeChild(child);
    }
    mNodeMap.clear();
}
