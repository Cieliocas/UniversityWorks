import type React from "react"
import Link from "next/link"
import { Button } from "@/src/components/ui/button"
import { FuriaLogo } from "../furia-logo"
import { Home, User, FileQuestion, Activity, Settings, LogOut, Menu } from "lucide-react"
import { Sheet, SheetContent, SheetTrigger } from "@/src/components/ui/sheet"

export function Sidebar() {
  return (
    <>
      {/* Mobile Sidebar */}
      <div className="lg:hidden fixed top-4 left-4 z-50">
        <Sheet>
          <SheetTrigger asChild>
            <Button variant="outline" size="icon" className="bg-gray-800 border-gray-700">
              <Menu className="h-5 w-5" />
            </Button>
          </SheetTrigger>
          <SheetContent side="left" className="w-64 p-0 bg-gray-900 border-gray-700">
            <SidebarContent />
          </SheetContent>
        </Sheet>
      </div>

      {/* Desktop Sidebar */}
      <div className="hidden lg:block w-64 bg-gray-900 border-r border-gray-800">
        <SidebarContent />
      </div>
    </>
  )
}

function SidebarContent() {
  return (
    <div className="h-full flex flex-col">
      <div className="p-4 flex items-center gap-2 border-b border-gray-800">
        <div className="w-8 h-8">
          <FuriaLogo />
        </div>
        <h2 className="text-xl font-bold text-white">Know Your Fan</h2>
      </div>

      <div className="flex-1 py-4 px-2 space-y-1">
        <SidebarLink href="/dashboard" icon={Home} label="Dashboard" active />
        <SidebarLink href="/dashboard/profile" icon={User} label="Meu Perfil" />
        <SidebarLink href="/dashboard/questionnaire" icon={FileQuestion} label="Questionário" />
        <SidebarLink href="/dashboard/activity" icon={Activity} label="Atividade" />
        <SidebarLink href="/dashboard/settings" icon={Settings} label="Configurações" />
      </div>

      <div className="p-4 border-t border-gray-800">
        <Button variant="ghost" className="w-full justify-start text-gray-400 hover:text-white hover:bg-gray-800">
          <LogOut className="mr-2 h-4 w-4" />
          Sair
        </Button>
      </div>
    </div>
  )
}

function SidebarLink({
  href,
  icon: Icon,
  label,
  active = false,
}: {
  href: string
  icon: React.ElementType
  label: string
  active?: boolean
}) {
  return (
    <Link href={href} className="block">
      <Button
        variant="ghost"
        className={`w-full justify-start ${
          active
            ? "bg-purple-900/30 text-purple-400 hover:bg-purple-900/40"
            : "text-gray-400 hover:text-white hover:bg-gray-800"
        }`}
      >
        <Icon className="mr-2 h-4 w-4" />
        {label}
      </Button>
    </Link>
  )
}
