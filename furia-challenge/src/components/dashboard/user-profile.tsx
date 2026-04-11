import { Card, CardContent, CardHeader, CardTitle } from "@/src/components/ui/card"
import { Avatar, AvatarFallback, AvatarImage } from "@/src/components/ui/avatar"
import { Button } from "@/src/components/ui/button"
import { Badge } from "@/src/components/ui/badge"
import { Edit } from "lucide-react"

export function UserProfile() {
  return (
    <Card className="bg-gray-800 border-gray-700 shadow-lg">
      <CardHeader className="pb-2">
        <div className="flex justify-between items-center">
          <CardTitle className="text-white">Meu Perfil</CardTitle>
          <Button variant="ghost" size="sm" className="text-gray-400 hover:text-white">
            <Edit className="h-4 w-4 mr-2" />
            Editar
          </Button>
        </div>
      </CardHeader>
      <CardContent>
        <div className="flex flex-col md:flex-row gap-6 items-center md:items-start">
          <Avatar className="h-24 w-24">
            <AvatarImage src="/placeholder.svg?height=96&width=96" alt="@user" />
            <AvatarFallback className="bg-purple-700 text-xl">FN</AvatarFallback>
          </Avatar>

          <div className="space-y-3 text-center md:text-left">
            <div>
              <h3 className="text-xl font-bold text-white">Fã da FURIA</h3>
              <p className="text-sm text-gray-400">Membro desde Abril 2024</p>
            </div>

            <div className="flex flex-wrap gap-2 justify-center md:justify-start">
              <Badge className="bg-purple-600">Super Torcedor</Badge>
              <Badge className="bg-gray-700">CS:GO</Badge>
              <Badge className="bg-gray-700">Valorant</Badge>
            </div>
          </div>
        </div>
      </CardContent>
    </Card>
  )
}
