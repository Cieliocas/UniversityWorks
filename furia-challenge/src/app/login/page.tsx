import { Button } from "@/src/components/ui/button"
import { Card, CardContent, CardDescription, CardFooter, CardHeader, CardTitle } from "@/src/components/ui/card"
import { Input } from "@/src/components/ui/input"
import { Label } from "@/src/components/ui/label"
import { Separator } from "@/src/components/ui/separator"
import Link from "next/link"
import { FuriaLogo } from "../../components/furia-logo"
import { SocialLoginButtons } from "@/src/components/social-login-buttons"

export default function LoginPage() {
  return (
    <main className="flex min-h-screen flex-col items-center justify-center bg-gradient-to-b from-black to-gray-900 p-4">
      <div className="w-full max-w-md">
        <div className="flex flex-col items-center mb-8">
          <div className="w-20 h-20 relative mb-4">
            <FuriaLogo />
          </div>
          <h1 className="text-2xl font-bold text-white">Entrar no Know Your Fan</h1>
        </div>

        <Card className="bg-gray-800 border-gray-700 shadow-xl">
          <CardHeader>
            <CardTitle className="text-white">Login</CardTitle>
            <CardDescription className="text-gray-400">Entre com sua conta para continuar</CardDescription>
          </CardHeader>
          <CardContent className="space-y-6">
            <div className="space-y-4">
              <div className="space-y-2">
                <Label htmlFor="email" className="text-gray-200">
                  Email
                </Label>
                <Input
                  id="email"
                  type="email"
                  placeholder="seu@email.com"
                  className="bg-gray-700 border-gray-600 text-white"
                />
              </div>
              <div className="space-y-2">
                <div className="flex items-center justify-between">
                  <Label htmlFor="password" className="text-gray-200">
                    Senha
                  </Label>
                  <Link href="/forgot-password" className="text-xs text-purple-400 hover:text-purple-300">
                    Esqueceu a senha?
                  </Link>
                </div>
                <Input
                  id="password"
                  type="password"
                  placeholder="••••••••"
                  className="bg-gray-700 border-gray-600 text-white"
                />
              </div>
            </div>

            <Button className="w-full bg-purple-600 hover:bg-purple-700">Entrar</Button>

            <div className="flex items-center gap-2 text-xs">
              <Separator className="flex-1 bg-gray-700" />
              <span className="text-gray-400">OU CONTINUE COM</span>
              <Separator className="flex-1 bg-gray-700" />
            </div>

            <SocialLoginButtons />
          </CardContent>
          <CardFooter className="flex justify-center">
            <p className="text-sm text-gray-400">
              Não tem uma conta?{" "}
              <Link href="/register" className="text-purple-400 hover:text-purple-300">
                Criar conta
              </Link>
            </p>
          </CardFooter>
        </Card>
      </div>
    </main>
  )
}
