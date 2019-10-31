#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <string>

#define LGW_VOID inline void

class lgw {
  struct ScreenBuffer {
    bool isModified = false;
    int nWidth_, nHeight_;
    sf::Uint8*  aBuffer; // Data array
    sf::Texture tBuffer; // Texture
    sf::Sprite* sBuffer = new sf::Sprite(tBuffer);

    sf::Sprite* getSprite() { return this->sBuffer; }

    void update() { this->tBuffer.update(this->aBuffer); }

    void set(int i, sf::Color color) {
      this->aBuffer[i+0] = (sf::Uint8)color.r;
      this->aBuffer[i+1] = (sf::Uint8)color.g;
      this->aBuffer[i+2] = (sf::Uint8)color.b;
      this->aBuffer[i+3] = (sf::Uint8)color.a;
    }

    void draw(int x, int y, sf::Color color) {
      int i = x*4+(y*nWidth_*4);
      this->set(i, color);
      this->update();
      isModified = true;
    }

    void clear(sf::Color color = sf::Color(0, 0, 0, 0xff)) {
      int nSize = (nWidth_ * nHeight_) * 4;
      for (int i = 0; i < nSize; i += 4) {
        this->set(i, color);
      }
      this->update();
      isModified = true;
    }

    ScreenBuffer(int nWidth, int nHeight) {
      this->nWidth_ = nWidth;
      this->nHeight_ = nHeight;
      int nSize = (nWidth_ * nHeight_) * 4;
      this->aBuffer = new sf::Uint8[nSize];
      this->tBuffer.create(nWidth, nHeight);
      this->sBuffer = new sf::Sprite(this->tBuffer);
      this->clear();
    }
  };




	std::shared_ptr <sf::RenderWindow> pWindow; // Pointer to a window
	std::shared_ptr <sf::Thread>	     pThread; // Pointer to a worker thread

  int nWidth, nHeight;

  ScreenBuffer sBuffer = ScreenBuffer(nWidth, nHeight);

	static inline void fRender(lgw* pContext) {
    ScreenBuffer& sBuffer = pContext->getBuffer();

    // Reactivate the window in this thread
		pContext->getWindow()->setActive(true);

    // Rendering loop
    while (pContext->getWindow()->isOpen()) {
        if (sBuffer.isModified) {
          sBuffer.update();
          pContext->getWindow()->draw(*sBuffer.getSprite());
          sBuffer.isModified = false;
        }
		    pContext->onDraw();
    }
	}

	inline void fEvents() {
		#define pWindow this->pWindow.get()
		while (pWindow->isOpen()) {
			sf::Event sEvent;
			while (pWindow->pollEvent(sEvent)) {
				switch (sEvent.type) {
					case sf::Event::Closed:
						this->onClose();
						this->pThread.get()->terminate();
					break;
				}
			}
		}
		#undef pWindow
	}

public:
	sf::RenderWindow* getWindow() { return this->pWindow.get(); }
  ScreenBuffer& getBuffer() { return this->sBuffer; }

	lgw() = default;

	inline void init(int _Width, int _Height, std::string _Title = "Untitled") {
    this->nWidth  = _Width;
    this->nHeight = _Height;

    // Initialize the window
    this->pWindow = std::make_shared <sf::RenderWindow>(sf::VideoMode(_Width, _Height), _Title);

    // Initialize the thread with a reference to the current object
    this->pThread = std::make_shared <sf::Thread>(&lgw::fRender, this);

    // Deactivate the window in this thread (reactivate later)
		this->pWindow.get()->setActive(false);

    // Launch the rendering thread
		this->pThread->launch();

    // Launch the events loop (this thread)
		this->fEvents();
	}

	inline bool isFocused() { return this->pWindow.get()->hasFocus(); }
	inline bool isOpen	 () { return this->pWindow.get()->isOpen(); }
	inline void clear	 (sf::Color _Color) { this->pWindow.get()->clear(_Color); }
	inline void update	 () { this->pWindow.get()->display(); }
	inline void close	 () { this->pWindow.get()->close(); }

	virtual void onDraw() {}
	virtual void onClose() {}
};
