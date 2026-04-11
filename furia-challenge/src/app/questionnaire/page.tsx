import { Button } from "@/src/components/ui/button"
import { Card, CardContent, CardDescription, CardFooter, CardHeader, CardTitle } from "@/src/components/ui/card"
import { Progress } from "@/src/components/ui/progress"
import { RadioGroup, RadioGroupItem } from "@/src/components/ui/radio-group"
import { Label } from "@/src/components/ui/label"
import { FuriaLogo } from "../../components/furia-logo"
import Link from "next/link"

export default function QuestionnairePage() {
  return (
    <main className="min-h-screen bg-gradient-to-b from-black to-gray-900 p-4">
      <div className="container max-w-3xl mx-auto py-8">
        <div className="flex flex-col items-center mb-8 text-center">
          <div className="w-16 h-16 relative mb-4">
            <FuriaLogo />
          </div>
          <h1 className="text-3xl font-bold text-white">Questionário do Fã</h1>
          <p className="text-gray-400 mt-2">
            Responda às perguntas abaixo para descobrirmos seu perfil como fã da FURIA
          </p>
        </div>

        <div className="mb-8">
          <div className="flex justify-between text-sm mb-2">
            <span className="text-gray-400">Progresso</span>
            <span className="text-purple-400">Pergunta 3 de 10</span>
          </div>
          <Progress value={30} className="h-2 bg-gray-700" indicatorClassName="bg-purple-500" />
        </div>

        <Card className="bg-gray-800 border-gray-700 shadow-xl mb-6">
          <CardHeader>
            <CardTitle className="text-white">Qual jogo da FURIA você mais acompanha?</CardTitle>
            <CardDescription className="text-gray-400">
              Escolha a opção que melhor representa sua preferência
            </CardDescription>
          </CardHeader>
          <CardContent>
            <RadioGroup defaultValue="cs2">
              <div className="space-y-3">
                <div className="flex items-center space-x-2">
                  <RadioGroupItem value="cs2" id="cs2" className="border-purple-400 text-purple-400" />
                  <Label htmlFor="cs2" className="text-white">
                    Counter-Strike 2
                  </Label>
                </div>
                <div className="flex items-center space-x-2">
                  <RadioGroupItem value="valorant" id="valorant" className="border-purple-400 text-purple-400" />
                  <Label htmlFor="valorant" className="text-white">
                    Valorant
                  </Label>
                </div>
                <div className="flex items-center space-x-2">
                  <RadioGroupItem value="lol" id="lol" className="border-purple-400 text-purple-400" />
                  <Label htmlFor="lol" className="text-white">
                    League of Legends
                  </Label>
                </div>
                <div className="flex items-center space-x-2">
                  <RadioGroupItem value="apex" id="apex" className="border-purple-400 text-purple-400" />
                  <Label htmlFor="apex" className="text-white">
                    Apex Legends
                  </Label>
                </div>
                <div className="flex items-center space-x-2">
                  <RadioGroupItem value="outros" id="outros" className="border-purple-400 text-purple-400" />
                  <Label htmlFor="outros" className="text-white">
                    Outros
                  </Label>
                </div>
              </div>
            </RadioGroup>
          </CardContent>
          <CardFooter className="flex justify-between">
            <Button variant="outline" className="border-gray-600 text-gray-400 hover:bg-gray-700">
              Anterior
            </Button>
            <Button className="bg-purple-600 hover:bg-purple-700">Próxima</Button>
          </CardFooter>
        </Card>

        <div className="text-center">
          <Button variant="link" asChild className="text-gray-400 hover:text-gray-300">
            <Link href="/dashboard">Salvar e continuar depois</Link>
          </Button>
        </div>
      </div>
    </main>
  )
}
