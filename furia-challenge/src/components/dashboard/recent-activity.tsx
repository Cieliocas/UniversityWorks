import type React from "react"
import { Card, CardContent, CardHeader, CardTitle } from "@/src/components/ui/card"
import { Button } from "@/src/components/ui/button"
import { Activity, MessageSquare, ThumbsUp, Eye } from "lucide-react"

export function RecentActivity() {
  return (
    <Card className="bg-gray-800 border-gray-700 shadow-lg">
      <CardHeader className="pb-2">
        <div className="flex justify-between items-center">
          <CardTitle className="text-white flex items-center gap-2">
            <Activity className="h-5 w-5 text-purple-400" />
            Atividade Recente
          </CardTitle>
          <Button variant="link" className="text-purple-400 hover:text-purple-300 p-0">
            Ver tudo
          </Button>
        </div>
      </CardHeader>
      <CardContent>
        <div className="space-y-4">
          <ActivityItem
            icon={Eye}
            title="Você visualizou um post da FURIA"
            description="FURIA vs. Team Liquid - Highlights"
            time="2 horas atrás"
          />

          <ActivityItem
            icon={ThumbsUp}
            title="Você curtiu um post da FURIA"
            description="Novo uniforme da equipe de CS:GO"
            time="1 dia atrás"
          />

          <ActivityItem
            icon={MessageSquare}
            title="Você comentou em um post"
            description="'Vamos FURIA! Rumo ao Major!'"
            time="3 dias atrás"
          />

          <ActivityItem
            icon={Eye}
            title="Você completou o questionário"
            description="Perfil de fã atualizado"
            time="1 semana atrás"
          />
        </div>
      </CardContent>
    </Card>
  )
}

function ActivityItem({
  icon: Icon,
  title,
  description,
  time,
}: {
  icon: React.ElementType
  title: string
  description: string
  time: string
}) {
  return (
    <div className="flex gap-3">
      <div className="mt-0.5">
        <div className="bg-gray-700 p-2 rounded-full">
          <Icon className="h-4 w-4 text-purple-400" />
        </div>
      </div>

      <div className="space-y-1">
        <p className="text-sm font-medium text-white">{title}</p>
        <p className="text-xs text-gray-400">{description}</p>
        <p className="text-xs text-gray-500">{time}</p>
      </div>
    </div>
  )
}
