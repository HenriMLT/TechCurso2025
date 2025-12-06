function procurarcep (){ //função que irá requisitar informações, receber json e colocar a informação no campo certo
    let cep = document.getElementById('cep').value; //irá pegar o cep digitado no input do cep
    if (cep !== ""){ //se o campo do cep for diferente de vazio, irá realizar a busca
        let url = "https://brasilapi.com.br/api/cep/v1/" + cep; //irá concatenar com o cep que foi digitado no input do cep
        
        let req = new XMLHttpRequest(); //requisição
        req.open("GET", url); //irá abrir a requisição, usando o método GET que é expecificado no brasilapi, consumindo a variável url
        req.send(); //irá enviar a requisição, consumindo a url

        req.onload = function(){ //irá tratar a resposta da requisição, pegando os dados recebidos via json e colocando nos campos corretos
            if(req.status === 200){
                let procurarendereco = JSON.parse(req.response); //irá acessar a resposta da aquisição e distribuir para os campos
                document.getElementById("rua").value = procurarendereco.street;
                document.getElementById("bair").value = procurarendereco.neighborhood; 
                document.getElementById("cid").value = procurarendereco.city; 
                document.getElementById("est").value = procurarendereco.state;                
            }
            else if(req.status === 404){
                alert("CEP incorreto");
            }
            else{
                alert("Erro, atualize a página");
            }
        }
    }
}

window.onload = function(){ //quando irá acontecer a função procurarcep
    let txtcep = document.getElementById("cep");
    txtcep.addEventListener("blur", procurarcep); //ele irá realizar a função ou evento procurarCEP quando sair ou ir para o próximo campo
}
