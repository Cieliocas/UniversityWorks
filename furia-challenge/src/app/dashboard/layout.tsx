import type React from "react"
import { Sidebar } from "@/src/components/dashboard/sidebar"
import { Header } from "@/src/components/dashboard/header"

export default function DashboardLayout({
  children,
}: {
  children: React.ReactNode
}) {
  return (
    <div className="min-h-screen bg-black">
      <div className="flex h-screen overflow-hidden">
        <Sidebar />
        <div className="flex-1 flex flex-col overflow-hidden">
          <Header />
          <main className="flex-1 overflow-auto bg-gradient-to-b from-gray-900 to-black p-4">{children}</main>
        </div>
      </div>
    </div>
  )
}
