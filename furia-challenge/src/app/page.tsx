import { Button } from "@/src/components/ui/button";
import {
  Card,
  CardContent,
  CardDescription,
  CardFooter,
  CardHeader,
  CardTitle,
} from "@/src/components/ui/card";
import Link from "next/link";
import { FuriaLogo } from "@/src/components/furia-logo";

export default function Home() {
  return (
    <main className="flex min-h-screen flex-col items-center justify-center bg-gradient-to-b from-black to-gray-900 p-4">
      <div className="container max-w-6xl mx-auto flex flex-col items-center gap-8">
        <div className="flex flex-col items-center gap-4 text-center">
          <div className="w-32 h-32 relative mb-4">
            <FuriaLogo />
          </div>
          <h1 className="text-4xl md:text-6xl font-bold text-white">
            Know Your <span className="text-purple-500">Fan</span>
          </h1>
          <p className="text-xl text-gray-300 max-w-2xl">
            Descubra seu perfil como fã da FURIA e ajude-nos a conhecer melhor
            nossa comunidade.
          </p>
        </div>

        <div className="grid grid-cols-1 md:grid-cols-2 gap-6 w-full max-w-4xl">
          <Card className="bg-gray-800 border-gray-700 shadow-xl">
            <CardHeader>
              <CardTitle className="text-white">Novo por aqui?</CardTitle>
              <CardDescription className="text-gray-400">
                Crie sua conta e descubra seu perfil de fã
              </CardDescription>
            </CardHeader>
            <CardContent className="flex flex-col gap-4">
              <p className="text-gray-300">
                Responda a um questionário interativo, conecte suas redes
                sociais e descubra que tipo de fã da FURIA você é!
              </p>
            </CardContent>
            <CardFooter>
              <Button
                asChild
                className="w-full bg-purple-600 hover:bg-purple-700"
              >
                <Link href="/register">Criar Conta</Link>
              </Button>
            </CardFooter>
          </Card>

          <Card className="bg-gray-800 border-gray-700 shadow-xl">
            <CardHeader>
              <CardTitle className="text-white">Já tem uma conta?</CardTitle>
              <CardDescription className="text-gray-400">
                Faça login para continuar sua jornada
              </CardDescription>
            </CardHeader>
            <CardContent className="flex flex-col gap-4">
              <p className="text-gray-300">
                Continue de onde parou, atualize seu perfil e veja como sua
                relação com a FURIA evolui ao longo do tempo.
              </p>
            </CardContent>
            <CardFooter>
              <Button
                asChild
                variant="outline"
                className="w-full border-purple-600 text-purple-400 hover:bg-purple-900/20"
              >
                <Link href="/login">Entrar</Link>
              </Button>
            </CardFooter>
          </Card>
        </div>

        <div className="mt-12 grid grid-cols-1 md:grid-cols-3 gap-6 w-full max-w-4xl">
          <FeatureCard
            title="Perfil Personalizado"
            description="Descubra que tipo de fã você é com base em suas preferências e comportamento."
          />
          <FeatureCard
            title="Conecte Redes Sociais"
            description="Integre suas contas para uma análise mais completa do seu perfil."
          />
          <FeatureCard
            title="Questionário Interativo"
            description="Responda perguntas sobre seus jogos, jogadores e momentos favoritos."
          />
        </div>
      </div>
    </main>
  );
}

function FeatureCard({
  title,
  description,
}: {
  title: string;
  description: string;
}) {
  return (
    <Card className="bg-gray-800/50 border-gray-700">
      <CardHeader>
        <CardTitle className="text-white text-lg">{title}</CardTitle>
      </CardHeader>
      <CardContent>
        <p className="text-gray-300 text-sm">{description}</p>
      </CardContent>
    </Card>
  );
}
