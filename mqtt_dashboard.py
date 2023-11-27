import re
import dash
import dash_core_components as dcc
import dash_html_components as html
import dash_bootstrap_components as dbc
import math
from datetime import datetime

external_stylesheets = ["./assets/style.css"]
app = dash.Dash(__name__, external_stylesheets=external_stylesheets)

# Calcula a sensação térmica


def calculate_sensacao_termica(temperatura, umidade):
    c = [-42.379, 2.04901523, 10.14333127, -0.22475541, -
         6.83783e-03, -5.481717e-02, 1.22874e-03, 8.5282e-04, -1.99e-06]

    # Converte a temperatura para Fahrenheit
    # Fórmula de Rothfusz
    temperatura_fahrenheit = (temperatura * 9/5) + 32

    # Calcula o Índice de Calor
    heat_index = c[0] + c[1]*temperatura_fahrenheit + c[2]*umidade + \
        c[3]*temperatura_fahrenheit*umidade + c[4]*(temperatura_fahrenheit**2) + \
        c[5]*(umidade**2) + c[6]*(temperatura_fahrenheit**2)*umidade + \
        c[7]*temperatura_fahrenheit * \
        (umidade**2) + c[8]*(temperatura_fahrenheit**2)*(umidade**2)

    # Converte o Índice de Calor de Fahrenheit para Celsius
    heat_index_celsius = (heat_index - 32) * 5/9
    return heat_index_celsius


def get_data(path):
    # Lista para armazenar os resultados
    resultados = []

    # Expressão regular para extrair timestamp, temperatura e umidade
    padrao = re.compile(r'\{([^}]+)\}')

    # Leitura do arquivo
    with open(path, 'r') as arquivo:
        linhas = arquivo.readlines()

        # Processa cada linha do arquivo
        for linha in linhas:
            # Tenta fazer o match com o padrão
            match = padrao.findall(linha)

            # Armazena os resultados na lista
            resultados.append(
                {'data': match[0], 'temperatura': match[1], 'umidade': match[2]})

    return resultados


graph_style = {"flex": 1, "min-width": 700}

# Cria os gráficos
grafico_temperatura = dcc.Graph(
    id="grafico-temperatura",
    responsive=True,
    className="graph",
)

grafico_umidade = dcc.Graph(
    id="grafico-umidade",
    responsive=True,
    className="graph",
)

grafico_combinado = dcc.Graph(
    id="grafico-combinado",
    responsive=True,
    className="graph",
)


sidebar = html.Div(
    className="sidebar",
    children=[
        html.Span([html.H1("Dashboard"),
                   html.P("Medição de temperatura e umidade", style={'display': 'flex', 'justify-content': 'center'})], style={'display': 'flex', 'flex-direction': 'column', 'margin-bottom': '50%'}),
        html.Img(src="./assets/images/ifes_logo.png",
                 style={"width": "40%", 'position': 'absolute', 'bottom': '10%'}),
    ],
)

header = html.Div(html.H1("Dashboard"), className="header")

content = html.Div(
    className="content",
    children=[
        html.Div([
            html.Div(html.Div(grafico_temperatura, className="grafico")),
            html.Div(html.Div(grafico_umidade, className="grafico")),
        ], className="first_row"),
        html.Div([
            html.Div(html.Div(grafico_combinado, className="grafico")),
        ], className="second_row"),
    ]
)

app.layout = html.Div(
    children=[
        header, sidebar, content,
        dcc.Interval(id='interval_component', n_intervals=0, interval=1000),
    ], className="dash_container"
)


@app.callback(
    dash.Output("grafico-temperatura", "figure"),
    dash.Output("grafico-umidade", "figure"),
    dash.Output("grafico-combinado", "figure"),
    dash.Input('interval_component', 'n_intervals'),
)
def update_graph(n):
    # Atualiza os dados
    dados = get_data('mensagens.txt')

    data = [dado['data'] for dado in dados]
    data = [datetime.strptime(ts, "%d/%m/%y-%H:%M") for ts in data]

    temperatura = [int(dado['temperatura'].replace(' C', ''))
                   for dado in dados]
    umidade = [int(dado['umidade'].replace(' %', '')) for dado in dados]

    # Atualiza os gráficos
    grafico_temperatura = {
        "data": [
            {
                "x": data,
                "y": temperatura,
                "type": "line",
            }
        ],
        "layout": {
            "title": "Temperatura",
            "xaxis": {"title": "Timestamp"},
            "yaxis": {"title": "Temperatura (°C)"},
        },
    }

    grafico_umidade = {
        "data": [
            {
                "x": data,
                "y": umidade,
                "type": "line",
            }
        ],
        "layout": {
            "title": "Umidade",
            "xaxis": {"title": "Timestamp"},
            "yaxis": {"title": "Umidade (%)"},
        },
    }

    grafico_combinado = {
        "data": [
            {
                "x": data,
                "y": temperatura,
                "type": "bar",
                "name": "Temperatura",
            },
            {
                "x": data,
                "y": [
                    calculate_sensacao_termica(
                        temperatura[i], umidade[i])
                    for i in range(len(temperatura))
                ],
                "type": "line",
                "name": "Sensação Térmica",
            }
        ],
        "layout": {
            "title": "Temperatura e Sensação Térmica",
            "xaxis": {"title": "Timestamp"},
            "yaxis": {"title": "Temperatura (°C)"},
        },

    }

    return grafico_temperatura, grafico_umidade, grafico_combinado


if __name__ == "__main__":
    port = 8000
    print(f"servidor rodando em: http://172.19.144.23:{port}")
    app.run_server(debug=False, host="0.0.0.0", port=port)
