#include "collisionhandler.h"
#include "logger.h"

CollisionHandler::CollisionHandler()
{

}

CollisionHandler::~CollisionHandler()
{

}

void CollisionHandler::addBall(OctaBall* ball)
{
    balls.push_back(ball);
}

void CollisionHandler::addWall(Plane* wall)
{
    walls.push_back(wall);
}

void CollisionHandler::DetectCollision(std::vector<PhysicsObject*> pObjects)
{
    if (pObjects.size() > 1)
    {
        std::vector<std::vector<int>> tracker;
        // assume the radius is 1
        for (int i = 0; pObjects.size() > i; i++)
        {
            tracker.push_back(*new std::vector<int>());
            for (int j = 0; j <= i; j++)
                tracker[i].push_back(j);

            for (int j = 0; pObjects.size() > j; j++)
            {
                if (std::find(tracker[i].begin(), tracker[i].end(), j) != tracker[i].end())
                {
                    continue;
                }
                float distance = pObjects[i]->getPosition().distanceToPoint(pObjects[j]->getPosition());
//                mLogger->logText(std::to_string(distance));
                if (distance < pObjects[i]->getRadius())
                {
                    Collide(pObjects[i], pObjects[j]);
                }
            }
        }
    }
    else
    {
        // pObjects er ikke større en 1
    }
}

void CollisionHandler::Collide(PhysicsObject* object1, PhysicsObject* object2)
{
//    mLogger->logText("Collision!");

    // objektspecifike sprett (ball (1) or wall (0))
    // viss et objekt er ball (1) og den andre er en vegg (0)
    if ((object1->type() != object2->type()))
    {
        if (object1->type() == 0 && object2->type() == 1)
        {
            // Ville ha likt og testa om denne Casten funker
            BallWallCollision((OctaBall *) object2, (Plane *) object1);
        }
        else
        {
            BallWallCollision((OctaBall *) object1, (Plane *) object2);
        }
    }
//    else if ((object1->type() == 1 && object2->type() == 1))
//    {
//        BallBallCollision((OctaBall *) object1, (OctaBall *) object2);
//    }
//    mLogger->logText("New velocity is " + std::to_string(object1->getVelocity().y()) + " and " + std::to_string(object2->getVelocity().y()));
}

void CollisionHandler::BallWallCollision(OctaBall* ball, Plane* wall)
{
//    mLogger->logText("Collision!");
    // Obs! Collide-funksjonen må endre seg til å kunne regne ut avstanden fra ballen til det nærmeste punktet på planet
    ball->setVelocity(-2 * QVector3D().dotProduct(wall->getNormal(), ball->getVelocity()) * wall->getNormal() + ball->getVelocity());

    // Ballen får normalen til planet og går oppover like langt som den gikk ned i planet
    // Eks: Viss ballen gikk -0.3 enheter under planet, skal ballen nå være 0.3 enheter over planet, i retning mot normalen
    // x og y retning (i forhold til normalen) skal være den samme
    float distance = ball->getPosition().distanceToPoint(wall->getPosition()) /* HUGSA! Byt ut wall->getPosition() når du finn det næraste mellomromet mellom ballen og planet */ - ball->getRadius();
    ball->move(wall->getNormal() * (distance));

    // Uferdig, ballen skal etter kollisjon endre hastighetvektoren til å gå fra veggen, men uten å endre x,z fartene (i forhold til normalen)
//    ball->setVelocity(ball->getVelocity() * wallNormal); // Vil sikkert ikke virke
}

void CollisionHandler::BallBallCollision(OctaBall* ball0, OctaBall* ball1)
{
    // Likt som i BallWallCollision, skal ballene vekk fra ballen like langt som de gikk inn i hverandre
    // "Normalen" i dette tilfellet vil være fra ballen sitt senter til senteret i den andre ballen (.normalized())
    // da de støtte på hverandre, slik at kollisjonen er litt mer dynamisk enn med en vegg


    // Ballen skal også skifte hastighetvektor til å fortsete i retning fra kollisjonspunktet.
    // Jeg følger formlene som blir vist i 9.7.5 (hvor n er normalen funnet i forrige kommentar og d som er avstanden mellom ballene da de kolliderte)
    // og endrer hastighetsvektoren til summen
}

int CollisionHandler::type()
{
    return 2;
}
