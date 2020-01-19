#include "board.h"

#include <iostream>
#include <memory>
#include <algorithm>
#include <assert.h>
#include <ctime>

namespace Pandemic {

Board::Board()
{
#if 1
    unsigned int seed = time(0);
#else
    unsigned int seed = 1;
#endif
    std::cout << "seed: " << seed << "\n";
    srand(seed);

    init();
}

void Board::reset()
{
    m_playerDeck.clear();
    m_playerDiscardPile.clear();
    m_infectionDeck.clear();
    m_infectionDiscardPile.clear();
    m_players.clear();
    m_cities.clear();
    m_mainCity.reset();
    m_cures.clear();
    m_numOutbreaks = 0;
    m_infectionRateIndex = 0;
}

void Board::init()
{
    initCures();
    initCities();
    initInfections();
    insertEventCards();
    initPlayers(2);
    distributePlayerCards();
    insertEpidemicCards(4);
}

void Board::initCures()
{
    m_cures.emplace_back(DiseaseType::Yellow);
    m_cures.emplace_back(DiseaseType::Red);
    m_cures.emplace_back(DiseaseType::Blue);
    m_cures.emplace_back(DiseaseType::Black);
}

void Board::initCities()
{
#if 0
    // Todo: Parse from file
    auto algiers = std::make_shared<City>("Algiers", DiseaseType::Black);
    auto atlanta = std::make_shared<City>("Atlanta", DiseaseType::Blue);
    auto baghdad = std::make_shared<City>("Baghdad", DiseaseType::Black);
    auto bangkok = std::make_shared<City>("Bangkok", DiseaseType::Red);
    auto bejing = std::make_shared<City>("Beijing", DiseaseType::Red);
    auto bogota = std::make_shared<City>("Bogota", DiseaseType::Yellow);
    auto buenosAries = std::make_shared<City>("Buenos Aries", DiseaseType::Yellow);
    auto cairo = std::make_shared<City>("Cairo", DiseaseType::Black);
    auto chenai = std::make_shared<City>("Chennai", DiseaseType::Black);
    auto chicago = std::make_shared<City>("Chicago", DiseaseType::Blue);
    auto delhi = std::make_shared<City>("Delhi", DiseaseType::Black);
    auto essen = std::make_shared<City>("Essen", DiseaseType::Blue);
    auto hoChiMing = std::make_shared<City>("Ho Chi Minh City", DiseaseType::Red);
    auto hongKong = std::make_shared<City>("Hong Kong", DiseaseType::Red);
    auto istanbul = std::make_shared<City>("Istanbul", DiseaseType::Black);
    auto jakarta = std::make_shared<City>("Jakarta", DiseaseType::Red);
    auto johannesburg = std::make_shared<City>("Johannesburg", DiseaseType::Yellow);
    auto karachi = std::make_shared<City>("Karachi", DiseaseType::Black);
    auto khartoum = std::make_shared<City>("Khartoum", DiseaseType::Yellow);
    auto kingshasa = std::make_shared<City>("Kinshasa", DiseaseType::Yellow);
    auto kolkata = std::make_shared<City>("Kolkata", DiseaseType::Black);
    auto lagos = std::make_shared<City>("Lagos", DiseaseType::Yellow);
    auto lima = std::make_shared<City>("Lima", DiseaseType::Yellow);
    auto london = std::make_shared<City>("London", DiseaseType::Blue);
    auto losAngeles = std::make_shared<City>("Los Angeles", DiseaseType::Yellow);
    auto madrid = std::make_shared<City>("Madrid", DiseaseType::Blue);
    auto manila = std::make_shared<City>("Manila", DiseaseType::Red);
    auto mexicoCity = std::make_shared<City>("Mexico City", DiseaseType::Yellow);
    auto miami = std::make_shared<City>("Miami", DiseaseType::Yellow);
    auto milan = std::make_shared<City>("Milan", DiseaseType::Blue);
    auto montreal = std::make_shared<City>("Montreal", DiseaseType::Blue);
    auto moscow = std::make_shared<City>("Moscow", DiseaseType::Black);
    auto mumbai = std::make_shared<City>("Mumbai", DiseaseType::Black);
    auto newYork = std::make_shared<City>("New York", DiseaseType::Blue);
    auto osaka = std::make_shared<City>("Osaka", DiseaseType::Red);
    auto paris = std::make_shared<City>("Paris", DiseaseType::Blue);
    auto riyadh = std::make_shared<City>("Riyadh", DiseaseType::Black);
    auto sanFrancisco = std::make_shared<City>("San Francisco", DiseaseType::Blue);
    auto santiago = std::make_shared<City>("Santiago", DiseaseType::Yellow);
    auto saoPaulo = std::make_shared<City>("Sao Paulo", DiseaseType::Yellow);
    auto seoul = std::make_shared<City>("Seoul", DiseaseType::Red);
    auto shanghai = std::make_shared<City>("Shanghai", DiseaseType::Red);
    auto stPetersburg = std::make_shared<City>("St. Petersburg", DiseaseType::Blue);
    auto sydney = std::make_shared<City>("Sydney", DiseaseType::Red);
    auto taipei = std::make_shared<City>("Taipei", DiseaseType::Red);
    auto tehran = std::make_shared<City>("Tehran", DiseaseType::Black);
    auto tokyo = std::make_shared<City>("Tokyo", DiseaseType::Red);
    auto washington = std::make_shared<City>("Washington", DiseaseType::Blue);

    addCity(algiers);
    addCity(atlanta);
    addCity(baghdad);
    addCity(bangkok);
    addCity(bejing);
    addCity(bogota);
    addCity(buenosAries);
    addCity(cairo);
    addCity(chenai);
    addCity(chicago);
    addCity(delhi);
    addCity(essen);
    addCity(hoChiMing);
    addCity(hongKong);
    addCity(istanbul);
    addCity(jakarta);
    addCity(johannesburg);
    addCity(karachi);
    addCity(khartoum);
    addCity(kingshasa);
    addCity(kolkata);
    addCity(lagos);
    addCity(lima);
    addCity(london);
    addCity(losAngeles);
    addCity(madrid);
    addCity(manila);
    addCity(mexicoCity);
    addCity(miami);
    addCity(milan);
    addCity(montreal);
    addCity(moscow);
    addCity(mumbai);
    addCity(newYork);
    addCity(osaka);
    addCity(paris);
    addCity(riyadh);
    addCity(sanFrancisco);
    addCity(santiago);
    addCity(saoPaulo);
    addCity(seoul);
    addCity(shanghai);
    addCity(stPetersburg);
    addCity(sydney);
    addCity(taipei);
    addCity(tehran);
    addCity(tokyo);
    addCity(washington);

    // Todo: Add neighbours to all cities
    santiago->addNeighbour(lima);
    lima->addNeighbour(bogota);
    lima->addNeighbour(mexicoCity);
    mexicoCity->addNeighbour(chicago);

    setStartCity(atlanta);
}

void Board::addCity(std::shared_ptr<City> city)
{
    m_playerDeck.push_back(std::make_shared<PlayerCityCard>(city));
    m_infectionDeck.push_back(std::make_shared<InfectionCard>(city));
    m_cities.push_back(city);
}

void Board::setStartCity(std::shared_ptr<City> city)
{
    city->setHasResearchStation(true);
    m_mainCity = city;
}

void Board::initInfections()
{
    std::cout << "--- Initializing infections\n";
    assert(m_infectionDeck.size() > 3*3);
    std::random_shuffle(m_infectionDeck.begin(), m_infectionDeck.end());
    for (int numCubes = 3; numCubes != 0; numCubes--)
    {
        std::cout << numCubes << " cubes:\n";
        for (int cityIx = 0; cityIx < 3; cityIx++)
        {
            std::shared_ptr<InfectionCard> card = m_infectionDeck.front();
            m_infectionDeck.pop_front();
            assert(card);
            std::cout << " " << card->city->getName() << "\n";
            for (int i = 0; i < numCubes; i++)
            {
                card->city->addDisease();
            }
            m_infectionDiscardPile.push_front(card);
        }
    }
}

void Board::initPlayers(const int numPlayers)
{
    assert(numPlayers >= 2);
    assert(numPlayers <= 4);

    // Find start city
    auto startCity = std::find_if(m_cities.begin(), m_cities.end(),
                                  [&](const std::shared_ptr<City>& c)
    {
        return c->getName() == m_mainCity->getName();
    });
    assert(*startCity);

    for (int playerIx = 0; playerIx < numPlayers; playerIx++)
    {
        std::shared_ptr<Player> player = std::make_shared<Player>();
        player->setCurrentCity(*startCity);

        while (true)
        {
            Role role = static_cast<Role>(std::rand() % c_numRoles);

            auto roleCollision = std::find_if(m_players.begin(), m_players.end(),
                                              [&](const std::shared_ptr<Player>& p)
            {
                return p->getRole() == role;
            });

            if (roleCollision != m_players.end())
            {
                continue;
            }

            player->setRole(role);
            break;
        }

        m_players.push_back(player);
    }
}

void Board::insertEventCards()
{
    for (int i = 0; i < c_numEventCards; ++i)
    {
        const auto type = static_cast<EventCardType>(i);
        m_playerDeck.push_back(std::make_shared<EventCard>(type));
    }
}

void Board::distributePlayerCards()
{
    assert(m_players.size() <= 4);

    std::cout << "\n--- Distribute player cards\n";
    std::random_shuffle(m_playerDeck.begin(), m_playerDeck.end());
    for (const std::shared_ptr<Player> p : m_players)
    {
        /* Distribute cards to players.
         *  2 players: 4 each
         *  3 players: 3 each
         *  4 players: 2 each
         */
        std::cout << roleToString(p->getRole()) << " gets \n";
        for (size_t i = 0; i < 6 - m_players.size(); i++)
        {
            std::shared_ptr<PlayerCard> card = m_playerDeck.front();
            std::cout << " - " << card->getName() << "\n";
            m_playerDeck.pop_front();
            p->addCard(card);
        }
    }
}

void Board::insertEpidemicCards(const int numEpidemicCards)
{
    std::random_shuffle(m_playerDeck.begin(), m_playerDeck.end());

    const int piles = numEpidemicCards;
    const size_t deckSize = m_playerDeck.size();
    const size_t pileSize = deckSize / piles;
    size_t remainder = deckSize % piles;
    auto begin = m_playerDeck.begin();
    auto end = begin;

    for (int pileIx = 0; pileIx < piles; pileIx++)
    {
        end += (remainder > 0) ? (pileSize + !!(remainder--)) : pileSize;
        end = m_playerDeck.insert(end, std::make_shared<EpidemicCard>());
        end++;
        std::random_shuffle(begin, end);
        begin = end;
    }
}

}
