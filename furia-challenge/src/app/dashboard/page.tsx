import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/src/components/ui/card"
import { Progress } from "@/src/components/ui/progress"
import { Button } from "@/src/components/ui/button"
import { Badge } from "@/src/components/ui/badge"
import { UserProfile } from "@/src/components/dashboard/user-profile"
import { FanTypeCard } from "@/src/components/dashboard/fan-type-card"
import { SocialConnections } from "@/src/components/dashboard/social-connections"
import { RecentActivity } from "@/src/components/dashboard/recent-activity"

export default function DashboardPage() {
  return (
    <div className="container mx-auto space-y-6">
      <div className="flex flex-col md:flex-row gap-6">
        <div className="md:w-2/3 space-y-6">
          <UserProfile />

          <Card className="bg-gray-800 border-gray-700 shadow-lg">
            <CardHeader>
              <CardTitle className="text-white">Complete seu perfil</CardTitle>
              <CardDescription className="text-gray-400">
                Quanto mais informações você fornecer, mais preciso será seu perfil de fã
              </CardDescription>
            </CardHeader>
            <CardContent className="space-y-4">
              <div className="space-y-2">
                <div className="flex justify-between text-sm">
                  <span className="text-gray-300">Progresso do perfil</span>
                  <span className="text-purple-400">65%</span>
                </div>
                <Progress value={65} className="h-2 bg-gray-700" indicatorClassName="bg-purple-500" />
              </div>

              <div className="grid grid-cols-1 md:grid-cols-3 gap-4 pt-2">
                <ProfileTask title="Questionário" description="Complete o questionário de perfil" status="completed" />
                <ProfileTask
                  title="Redes Sociais"
                  description="Conecte pelo menos uma rede social"
                  status="in-progress"
                />
                <ProfileTask title="Preferências" description="Defina suas preferências de jogos" status="pending" />
              </div>
            </CardContent>
          </Card>

          <RecentActivity />
        </div>

        <div className="md:w-1/3 space-y-6">
          <FanTypeCard />
          <SocialConnections />
        </div>
      </div>
    </div>
  )
}

type TaskStatus = "completed" | "in-progress" | "pending"

function ProfileTask({
  title,
  description,
  status,
}: {
  title: string
  description: string
  status: TaskStatus
}) {
  const statusConfig = {
    completed: {
      badge: <Badge className="bg-green-600">Concluído</Badge>,
      buttonText: "Ver detalhes",
      buttonVariant: "outline" as const,
    },
    "in-progress": {
      badge: <Badge className="bg-amber-600">Em progresso</Badge>,
      buttonText: "Continuar",
      buttonVariant: "default" as const,
    },
    pending: {
      badge: <Badge className="bg-gray-600">Pendente</Badge>,
      buttonText: "Iniciar",
      buttonVariant: "default" as const,
    },
  }

  const config = statusConfig[status]

  return (
    <Card className="bg-gray-800/50 border-gray-700">
      <CardContent className="p-4 space-y-3">
        <div className="flex justify-between items-start">
          <div className="space-y-1">
            <h3 className="font-medium text-white">{title}</h3>
            <p className="text-xs text-gray-400">{description}</p>
          </div>
          {config.badge}
        </div>
        <Button
          variant={config.buttonVariant}
          size="sm"
          className={status === "pending" ? "bg-purple-600 hover:bg-purple-700 w-full" : "w-full"}
        >
          {config.buttonText}
        </Button>
      </CardContent>
    </Card>
  )
}
