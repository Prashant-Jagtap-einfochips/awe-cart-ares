#
# Helper script for creating tune files
#
#   BOSE CORPORATION
#   COPYRIGHT (c) 2007-2007 BOSE CORPORATION ALL RIGHTS RESERVED
#   This program may not be reproduced, in whole or in part in an
#   form or any means whatsoever without the written permission of:
#
#       BOSE CORPORATION
#       The Mountain
#       Framingham, MA 01701-9168
#

use warnings;
use strict;
use XML::LibXML;
use Data::Dumper;
use JSON;

my $debug = 0;
my $generate_atlas_base_tunefile = 1;

sub get_car_type {
    my $car_type = undef;
    
    if ($debug) 
    {
        $car_type = 'BATMOBILE';
    }
    else 
    {
        # Prompt user for car type
        print "Enter the car type name: ";
        $car_type = <STDIN>;
        chomp $car_type;
    }
    
    return $car_type;
}

sub get_mode_list {
    my @mode_list;

    if ($debug)
    {
        @mode_list = ('driving', 'parked');
    }
    else
    {
        # Prompt user for list of modes
        print "Enter the mode names separated by spaces: ";
        my $modes = <STDIN>;
        chomp $modes;
        @mode_list = split / /, $modes;
    }
    
    return @mode_list;
}

sub get_master_tune_list {
    my $master_tune_list = undef;

    if ($debug)
    {
        $master_tune_list = './work/FancyModel_TuneVar.xml';
    }
    else
    {
        # Prompt user for master tune list file
        print "Enter master tune list filename: ";
        $master_tune_list = <STDIN>;
        chomp $master_tune_list;
    }

    # Make sure file exists
    if (-f $master_tune_list) {}
    else {
        die "Couldnt find that file\n";
    }
    
    return $master_tune_list;
}

sub add_tune_parameters_to_element {
    # get inputs
    my $tune_parameters = shift;
    my $element = shift;
    my $dom = shift;

    # Loop through the tune parameters and add them to that element
    my $tune_parameter = undef;
    foreach $tune_parameter (@$tune_parameters)
    {
        # Get the name and value
        my $name = $tune_parameter->findvalue('./@id');
        my $value = $tune_parameter->findvalue('./DefaultValue');
        my @data = split / /, $value;
            
        # Create the element, but dont add yet
        my $tune_element = $dom->createElement('TUNE_PARAMETER');
        $tune_element->setAttribute('id'=>$name);
        $tune_element->appendText($value);
        $element->appendChild($tune_element);
    }
        
}

sub Write_Data_to_JSON
{
   my $data = shift;
   my $filename = shift;
   
   print "Writing Data to file: $filename\n";
   open my $fh, ">", $filename;
   my $jsonizer = JSON->new->pretty;
   print $fh $jsonizer->encode($data);
   close $fh;
}

sub create_atlasbase_tunefile
{
    my $tune_data = shift;
    my $feature = shift;
    my $car_type = shift;
    if ($generate_atlas_base_tunefile)
    {
        my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time);
        my $timestamp = sprintf ( "%02d/%02d/%04d, %02d:%02d:%02d",
                                   $mday,$mon+1,$year+1900,$hour,$min,$sec);
        $tune_data->{"header"}{"version"} = "1.0.0";
        $tune_data->{"header"}{"timestamp"} = $timestamp;
        $tune_data->{"header"}{"variant"} = $car_type;
        $tune_data->{"header"}{"read_only"} = "false";

        my $filename = $feature . ".tune.atlasbase";
        Write_Data_to_JSON($tune_data, $filename);
    }
}

# Get all the inputs    
my $car_type = get_car_type();
my @mode_list = get_mode_list();
my $master_tune_list = get_master_tune_list();

# Open the master tune list and extract overlay groups
my $master_tune_dom = XML::LibXML->load_xml(location => $master_tune_list);
my @overlay_groups = $master_tune_dom->findnodes('/root/OverlayGroup');

# Loop through each overlay group and pull out name/value pairs
my %doms;
my $doms = \%doms;
my @all_tune_parameters;
foreach my $group (@overlay_groups)
{
    # Grab the overlay group name and package list
    my $group_name = $group->findvalue('./@id');
    print "GROUP: $group_name\n";
    
    # Loop through the packages in the master tune list
    my @packages = $group->findnodes('./Package');
    foreach my $package_element (@packages)
    {
        # Extract the package name
        my $package_name = $package_element->findvalue('./@Name');

        # Extract the tune parameters for this pacakge
        my @tune_parameters = $package_element->findnodes('./TuneVar');

        # Extract the Tuning tool feature group
        my $tool_group = $package_element->findvalue('./@ToolGroup');
        print "PCKG: $package_name\nTG: $tool_group\n";
        
        # Determine the feature name by choosing the tool group, or if empty
        # choose the overlay group
        my $feature_name;
        if ($tool_group eq "")
        {
            # Do everything as you used to. use group_name
            $feature_name = $group_name;
        }
        else
        {
            $feature_name = $tool_group;
        }
        print "FT: $feature_name\n";

        if ($generate_atlas_base_tunefile)
        {
            my %tune_data;
            foreach my $tune_parameter (@tune_parameters)
            {
                # Get the name and value
                my $name = $tune_parameter->findvalue('./@id');
                my $value = $tune_parameter->findvalue('./DefaultValue');
                my @data = split / /, $value;
                    
                # Create the data struct for atlasbase file
                $tune_data{$car_type}{$feature_name}{"tunedata"}{$name} = \@data;
            }
            create_atlasbase_tunefile(\%tune_data, $feature_name, $car_type);
        }
        else
        {
            # Figure out if the file exists or not
            my ($dom, $root, $car_type_element);
            if (exists $doms->{$feature_name})
            {
                print "Found an existing dom\n";
                
                $dom = $doms->{$feature_name};
                $root = $dom->documentElement();
                my @cts = $root->findnodes('./CARTYPE');
                $car_type_element = pop @cts;
            }
            else
            {
                print "Adding new dom object to the hash\n";
                
                # Create a new dom object
                $dom = XML::LibXML::Document->createDocument("1.0", "UTF-8");
                $root = $dom->createElement('root');
                $root->setAttribute('feature'=>$feature_name);
                $dom->setDocumentElement($root);

                # Create the cartype level
                $car_type_element = $dom->createElement('CARTYPE');
                $car_type_element->setAttribute('name'=>$car_type);
                $root->appendChild($car_type_element);

                # Add it to the doms list
                $doms{$feature_name} = $dom;
            }

            # Figure out which hierarchy to put these tune parameters
            add_tune_parameters_to_element(\@tune_parameters, $root, $dom);
            print "Adding global params\n";

            # Write all the DOMs to a file
            my @features = keys %doms;
            for my $feature (@features)
            {
                my $filename = $feature . ".tune.xml";
                my $tune_file_dom = $doms{$feature};
                open(my $fh, '>', $filename) or die "Could not open file $filename\n";
                print $fh $tune_file_dom->toString(1);
                close $fh;
            }
        }
    }
}

