#include "hzpch.h"
#include "LayerStack.h"

namespace Raying
{
	LayerStack::LayerStack()
	{
		_layerInsert = _layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : _layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		_layerInsert = _layers.emplace(_layerInsert, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		_layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto itr = std::find(_layers.begin(), _layers.end(), layer);
		if (itr != _layers.end())
		{
			_layers.erase(itr);
			_layerInsert--;
		}
	}

	void LayerStack::PopOverLay(Layer* overlay)
	{
		auto itr = std::find(_layers.begin(), _layers.end(), overlay);
		if (itr != _layers.end())
		{
			_layers.erase(itr);
		}

	}

}