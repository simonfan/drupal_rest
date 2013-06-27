<?php

class REST_EntityItem {
	// private properties
	private $entity;
	private $entity_type;
	private $required;

	// the object
	public $item = array();

	public function __construct($entity_type, $entity, $required) {
		$this->entity_type = $entity_type;
		$this->entity = $entity;

		// $required is an array containing 2 keys: fields and properties
		$this->get_properties( $required['properties'] );
		$this->get_fields( $required['fields'] );
	}


	private function get_properties($required_properties = NULL) {
		// get the required properties for the entity
		// and store them as public properties of the object
		if ( !empty($required_properties) ) {

			/*
			// check if in the required fields the nid is already requested.
			// if so, do nothing. Else, add the nid to the required properties
			if ( !isset($required_properties['nid']) ) {
				$required_properties['nid'] = array('alias' => 'nid');
			}
			*/

			foreach ($required_properties as $property_name => $options) {

				$alias = !empty($options['alias']) ? $options['alias'] : $property_name;

				$this->item[ $alias ] = $this->entity->{$property_name};
			}
		}
	}

	private function get_fields($fields_information = NULL) {
		if ( empty($fields_information) ) { return false; }

		// get the required fields for the entity and store 
		// them in public properties of the new object.
		foreach ($fields_information AS $field_name => $options) {
			// field_get_items just processes the value inside the $entity object. 
			// It does not do a request to the database, because the entity has already been loaded.
			// field_reader($name, $entity, $entity_type, $options = array(), $langcode = NULL) {
			$field_value = field_reader($field_name, $this->entity, $this->entity_type, $options);

			$alias = !empty($options['alias']) ? $options['alias'] : $field_name;

			$this->item[ $alias ] = $field_value;
		}
	}

	public function export() {
		return $this->item;
	}
}