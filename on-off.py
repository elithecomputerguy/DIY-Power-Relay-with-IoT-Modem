from bottle import route, run, request
import json 
import os

def modem(command):
    id_device = ''
    id_project = ''
    id_client = ''
    secret = ''
    query = f'''
            curl -X POST -L 'https://notehub.io/oauth2/token' \
            -H 'content-type: application/x-www-form-urlencoded' \
            -d grant_type=client_credentials -d client_id={id_client} \
            -d client_secret={secret}
            '''

    response = os.popen(query).read()
    response = json.loads(response)
    token = response['access_token']
    print(f'Token: {token}')

    query = f"""
            curl -X POST -L 'https://api.notefile.net/v1/projects/{id_project}/devices/{id_device}/notes/data.qi' \
            -H 'Authorization: Bearer {token}' \
            -d '{{\"body\":{{\"message\":\"{command}\"}}}}' 
            """
    
    os.system(query)

    print(f'Command was: {command}')

@route('/switch')
def switch():
    command = request.query.get('command')
    modem(command)

    page = '''
            <h1>Light Switch<h1>
            <a href="http://127.0.0.1/switch?command=on">ON</a>
            <a href="http://127.0.0.1/switch?command=off">OFF</a>
            <br>
            '''

    footer = f'light is {command}'

    page = f'{page} {footer}'

    return page

run(host='0.0.0.0', port=80, debug=True)