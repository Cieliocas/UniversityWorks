import { Card, CardContent, CardHeader, CardTitle } from "@/src/components/ui/card"
import { Button } from "@/src/components/ui/button"
import { Trophy } from "lucide-react"

export function FanTypeCard() {
  return (
    <Card className="bg-gradient-to-br from-purple-900 to-gray-900 border-gray-700 shadow-lg overflow-hidden">
      <CardHeader className="pb-2">
        <CardTitle className="text-white flex items-center gap-2">
          <Trophy className="h-5 w-5 text-yellow-400" />
          Seu Perfil de Fã
        </CardTitle>
      </CardHeader>
      <CardContent>
        <div className="space-y-4">
          <div className="text-center py-4">
            <h3 className="text-2xl font-bold text-white mb-1">Super Torcedor Estratégico</h3>
            <p className="text-sm text-gray-300">
              Você é um fã que valoriza a estratégia e a tática nos jogos, sempre analisando as jogadas e decisões da
              equipe.
            </p>
          </div>

          <div className="space-y-3">
            <FanAttribute name="Engajamento" value={85} />
            <FanAttribute name="Conhecimento" value={92} />
            <FanAttribute name="Lealdade" value={78} />
          </div>

          <Button className="w-full bg-purple-600 hover:bg-purple-700">Ver Análise Completa</Button>
        </div>
      </CardContent>
    </Card>
  )
}

function FanAttribute({ name, value }: { name: string; value: number }) {
  return (
    <div className="space-y-1">
      <div className="flex justify-between text-sm">
        <span className="text-gray-300">{name}</span>
        <span className="text-white font-medium">{value}%</span>
      </div>
      <div className="h-2 bg-gray-700 rounded-full overflow-hidden">
        <div className="h-full bg-purple-500 rounded-full" style={{ width: `${value}%` }} />
      </div>
    </div>
  )
}
