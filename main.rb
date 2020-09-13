require 'ipaddr'
require 'pry'
def get_not_inc_segment_prefix(target_network, host)
  return 32 if target_network == host
  (8..32).each do |prefix| # 32は含まれる
    address_range = target_network.mask(prefix).to_range
    unless target_network.mask(prefix).include? host
      puts "#{address_range.first.to_s}/#{prefix}(#{address_range.first.to_s}-#{address_range.last.to_s})"
      return prefix
    end
  end
  raise '想定外'
end

def print_not_inc_host_network(target_network, host)
  end_network = "11.0.0.0"
  # while target_network != host do
  while target_network.to_s != end_network do
    prefix = get_not_inc_segment_prefix(target_network, host)
    next_addr_int = target_network.mask(prefix).to_range.last.to_i + 1
    target_network = IPAddr.new("#{IPAddr.new(next_addr_int, Socket::AF_INET)}/#{prefix}")
  end
end

begin
  host = IPAddr.new ARGV.first
  target_network = IPAddr.new("10.0.0.0/8")
  print_not_inc_host_network(target_network, host)
rescue => e
  puts '入力が正しいかご確認ください。'
  p e.message
  p e.backtrace
end
