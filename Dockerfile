FROM debian:bullseye

EXPOSE 80

RUN apt-get update && apt-get upgrade -y && apt-get install vim nginx -y

RUN rm -f /etc/nginx/sites-enabled/default /etc/nginx/sites-available/default

COPY ./c.conf /etc/nginx/conf.d/

CMD ["sleep", "infinity"]