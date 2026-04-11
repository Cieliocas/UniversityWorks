import type React from "react"
import { Card, CardContent, CardHeader, CardTitle } from "@/src/components/ui/card"
import { Button } from "@/src/components/ui/button"
import { FaGoogle, FaTwitter, FaDiscord } from "react-icons/fa"

export function SocialConnections() {
  return (
    <Card className="bg-gray-800 border-gray-700 shadow-lg">
      <CardHeader className="pb-2">
        <CardTitle className="text-white">Redes Sociais</CardTitle>
      </CardHeader>
      <CardContent className="space-y-4">
        <p className="text-sm text-gray-400">
          Conecte suas redes sociais para uma análise mais completa do seu perfil de fã.
        </p>

        <div className="space-y-3">
          <SocialButton icon={FaGoogle} name="Google" status="connected" />
          <SocialButton icon={FaTwitter} name="Twitter" status="not-connected" />
          <SocialButton icon={FaDiscord} name="Discord" status="not-connected" />
        </div>
      </CardContent>
    </Card>
  )
}

type SocialStatus = "connected" | "not-connected"

function SocialButton({
  icon: Icon,
  name,
  status,
}: {
  icon: React.ElementType
  name: string
  status: SocialStatus
}) {
  return (
    <div className="flex items-center justify-between">
      <div className="flex items-center gap-3">
        <Icon className="h-5 w-5 text-gray-400" />
        <span className="text-gray-300">{name}</span>
      </div>

      {status === "connected" ? (
        <Button variant="outline" size="sm" className="border-gray-600 text-gray-400 hover:bg-gray-700">
          Conectado
        </Button>
      ) : (
        <Button size="sm" className="bg-purple-600 hover:bg-purple-700">
          Conectar
        </Button>
      )}
    </div>
  )
}
