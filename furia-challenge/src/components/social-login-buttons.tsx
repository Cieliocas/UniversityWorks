import { Button } from "@/src/components/ui/button"
import { FaGoogle, FaTwitter, FaDiscord } from "react-icons/fa"

export function SocialLoginButtons() {
  return (
    <div className="grid grid-cols-3 gap-3">
      <Button variant="outline" className="border-gray-600 hover:bg-gray-700 text-white">
        <FaGoogle className="mr-2 h-4 w-4" />
        Google
      </Button>
      <Button variant="outline" className="border-gray-600 hover:bg-gray-700 text-white">
        <FaTwitter className="mr-2 h-4 w-4" />
        Twitter
      </Button>
      <Button variant="outline" className="border-gray-600 hover:bg-gray-700 text-white">
        <FaDiscord className="mr-2 h-4 w-4" />
        Discord
      </Button>
    </div>
  )
}
