#include <iostream>
#include <vector>

/*
    Hola Nilo du coup voila le petit fichier !

    En gros après avoir cherché j'ai compris que pendant l'execution le serveur peut être ammené à avoir des comportements
    totalement différents en fonction du port sur lequel une requette est reçue et en fonction du repertoire(dossier) concerné par une requette.
    Au delà de certains champs comme le nom du serveur et l'IP, ces trucs la peuvent rendre le process super lent à analyser
    dans quel cas il est et du coup comment faire ceci cela..

    Du coup j'ai giga ramer mais j'ai mis cette structure la en place et elle devrait permettre de bien ranger toutes les infos
    et par conséquent d'avoir la vie facile à l'execution en evitant un deluge de if/else

    Pour le parsing on devra juste créer autant d'objets Sever qu'il y a de block dans le fichier de config, remplir les champs ou non
    et assigner les valeurs par defauts quand nécessaire, et tout pareil pour les blocks Locations !

    Je sais pas si j'ai oublié des trucs ou si un détail m'échappe mais si c'est le cas hesites pas à me le faire savoir !
    En attendant vois si ça te convient comme ça et si oui c'est top :)
*/

/*
    * NN = non NULL
    * Tout les champs existants dans un objet Location précisés dans l'objet Server auquel il appartient priment sur la valeur indiquée dans l'objet Location
    * Les champs non indiqués (NN) peuvent êtres nulls (non précisés dans le fichier de conf) mais certains doivent être assignés à une valeur par defaut dans ce cas (website_path, cgi_path, index...)
    * Souvent un block Location est utilisé pour la conf du dossier `cgi-bin`, d'où le fait qu'il-y-ait les champs `cgi-path` et `cgi-ext` dans la classe
    * J'ai pas précisé la nature des champs des classes (private, public) mais tu les trouvera toutes propres dans le dossier include d'ici peu
*/

// La classe core: correspond au scope de base du programe et qui contient les données non specifialbles
class Core
{
    const int           max_clients;                    // (NN) Le nombre de clients max en simultané
    
    std::vector<int>    server_sockets;                 // (NN) Un tableau de sockets pour chaque ports

    std::vector<Server> servers;                        // (NN) Un tableau d'objets de la class Server qui contiennent chacun leur config
    
    ...(fonctions);
}

// La classe Server: pour chaque instance prend ses données dans un block `server` different dans le fichier de conf (peut y avoir 1 seule instance)
class Server
{
    std::vector<int>            ports;                  // (NN) Un tableau de ports (il peut y avoir plusieurs ports dans un même block `server`)
    
    std::vector<std::string>    methods;                // Un tableau des methodes de requettes acceptées pour le serveur
    std::vector<std::string>    cgi_ext;                // Un tableau des extensions pour les fichiers CGI
    
    std::string                 root_path;              // Le chemin de la racine du serveur
    std::string                 server_name;            // Le nom du serveur
    std::string                 host;                   // (NN) L'IP du serveur
    std::string                 index;                  // Le fichier index à la racine du server
    std::string                 error_pages;            // Le chemin vers un fichier contenant des pages d'erreur (404, 400..) pouvant être ajouté
    std::string                 return_path;            // Un chemin de redirection ou d'erreur à renvoyer par defaut si precisé quand l'URI pointe sur le serveur
    std::string                 cgi_path;               // Le chemin vers le CGI qui executera les scripts (Python, Bash, PHP-CGI...)
    
    long                        client_max_body_size;   // () La taille max accepté par le serveur pour le body d'une requette client (exemple upload d'un fichier)
    
    bool                        autoindex;              // Le server fait du directory listing ou non

    std::vector<Location>       locations;              // Un tableau d'objets de la classe Location qui ont eu aussi chacun leur config

    ...(fonctions);
}

// La classe Location: au sein d'un objet Server, certains chemins peuvent avoir des configurations différentes (peut être NULL)
class Location
{
    std::vector<std::string>    methods;                // Un tableau des methodes de requettes acceptées pour cet endroit du serveur (ce repertoire)
    std::vector<std::string>    cgi_ext;                // Un tableau des extensions pour les fichiers CGI
    
    std::string                 root_path;              // Le chemin de la racine du repertoire
    std::string                 index;                  // Le fichier index à la racine du repertoire
    std::string                 return_path;            // Un chemin de redirection ou d'erreur à renvoyer par defaut si precisé quand l'URI pointe sur ce repertoire
    std::string                 cgi_path;               // Le chemin vers le CGI qui executera les scripts (Python, Bash, PHP-CGI...)
    
    bool                        autoindex;              // Le repertoire fait du directory listing ou non

    ...(fonctions);
}